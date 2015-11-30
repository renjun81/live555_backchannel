//
//  ADTSAudioFileSink.h
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#ifndef __live555_trace__ADTSAudioFileSink__
#define __live555_trace__ADTSAudioFileSink__

#ifndef _FILE_SINK_HH
#include "FileSink.hh"
#endif

class ADTSAudioFileSink: public FileSink {
public:
    static ADTSAudioFileSink* createNew(UsageEnvironment& env, char const* fileName,
                                        unsigned bufferSize = 10000,
                                        Boolean oneFilePerFrame = False);
    // (See "FileSink.hh" for a description of these parameters.)
    
protected:
    ADTSAudioFileSink(UsageEnvironment& env, FILE* fid, unsigned bufferSize,
                      char const* perFrameFileNamePrefix);
    // called only by createNew()
    virtual ~ADTSAudioFileSink();
    
protected: // redefined virtual functions:

    virtual Boolean sourceIsCompatibleWithUs(MediaSource& source);
    virtual void afterGettingFrame(unsigned frameSize,
                                   unsigned numTruncatedBytes,
                                   struct timeval presentationTime);
    static void afterGettingFrame(void* clientData, unsigned frameSize,
                                  unsigned numTruncatedBytes,
                                  struct timeval presentationTime,
                                  unsigned durationInMicroseconds);
                                  
private:
    // redefined virtual functions:
    virtual Boolean continuePlaying();
                                      
protected:
    Boolean fHaveWrittenHeader;
};

#endif /* defined(__live555_trace__ADTSAudioFileSink__) */
