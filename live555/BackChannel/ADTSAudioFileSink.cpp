//
//  ADTSAudioFileSink.cpp
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

// reference AMRAudioFileSink.cpp
#include "ADTSAudioFileSink.hh"
#include "OutputFile.hh"
extern "C"
{
    #include "AudioUtility.h"
}
////////// ADTSAudioFileSink //////////

ADTSAudioFileSink
::ADTSAudioFileSink(UsageEnvironment& env, FILE* fid, unsigned bufferSize,
                   char const* perFrameFileNamePrefix)
: FileSink(env, fid, bufferSize, perFrameFileNamePrefix),
fHaveWrittenHeader(False) {
}

ADTSAudioFileSink::~ADTSAudioFileSink() {
}

ADTSAudioFileSink*
ADTSAudioFileSink::createNew(UsageEnvironment& env, char const* fileName,
                            unsigned bufferSize, Boolean oneFilePerFrame) {
    do {
        FILE* fid;
        char const* perFrameFileNamePrefix;
        if (oneFilePerFrame) {
            // Create the fid for each frame
            fid = NULL;
            perFrameFileNamePrefix = fileName;
        } else {
            // Normal case: create the fid once
            fid = OpenOutputFile(env, fileName);
            if (fid == NULL) break;
            perFrameFileNamePrefix = NULL;
        }
        
        return new ADTSAudioFileSink(env, fid, bufferSize, perFrameFileNamePrefix);
    } while (0);
    
    return NULL;
}

Boolean ADTSAudioFileSink::sourceIsCompatibleWithUs(MediaSource& source) {
    // The input source must be a MPEG4GenericRTPSource
    return source.isRTPSource();
}

Boolean ADTSAudioFileSink::continuePlaying() {
    printf("ADTSAudioFileSink::continuePlaying() line=%d fBufferSize=%d\n\n\n", __LINE__, fBufferSize);
    if (fSource == NULL) return False;
    
    fSource->getNextFrame(fBuffer, fBufferSize,
                          afterGettingFrame, this,
                          onSourceClosure, this);
    
    return True;
}


void ADTSAudioFileSink::afterGettingFrame(void* clientData, unsigned frameSize,
                              unsigned numTruncatedBytes,
                              struct timeval presentationTime,
                              unsigned durationInMicroseconds)
{
    printf("ADTSAudioFileSink::afterGettingFrame line=%d \n", __LINE__);
    ADTSAudioFileSink* sink = (ADTSAudioFileSink*)clientData;
    sink->afterGettingFrame(frameSize, numTruncatedBytes, presentationTime);
}

void ADTSAudioFileSink::afterGettingFrame(unsigned frameSize,
                                         unsigned numTruncatedBytes,
                                         struct timeval presentationTime) {
    //AMRAudioSource* source = (AMRAudioSource*)fSource;
    //if (source == NULL) return; // sanity check
    printf("ADTSAudioFileSink::afterGettingFrame line=%d frameSize=%d\n", __LINE__, frameSize);

    fHaveWrittenHeader = True;

    tAACADTSHeaderInfo vxADTSHeader={0};
    unsigned char pADTSHeader[10]={0};
    unsigned char pInput[] = {0xff,0xf9,0x58,0x80,0,0x1f,0xfc};

    parseAACADTSString(pInput,&vxADTSHeader);
    vxADTSHeader.sampling_frequency_index = 11; //8000hz
    vxADTSHeader.channel_configuration = 1;
    vxADTSHeader.adts_buffer_fullness = 0x7ff;
    vxADTSHeader.frame_length = 7+frameSize;
    //printAACAHeader(&vxADTSHeader);

    generateAACADTSString(pADTSHeader,&vxADTSHeader);
    //printf("%s %s line=%d frameSize=%d\n",__FILE__, __FUNCTION__ , __LINE__, frameSize);
    fwrite(pADTSHeader, 1, 7, fOutFid);
;
    // Call the parent class to complete the normal file write with the input data:
    FileSink::afterGettingFrame(frameSize, numTruncatedBytes, presentationTime);
    
}
