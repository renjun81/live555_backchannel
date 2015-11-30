//
//  ADTSBackChannelAudioFileServerMediaSubsession.cpp
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#include "ADTSBackChannelAudioFileServerMediaSubsession.hh"
#include "MPEG4GenericRTPSource.hh"
#include "ADTSAudioFileSink.hh"

ADTSBackChannelAudioFileServerMediaSubsession*
ADTSBackChannelAudioFileServerMediaSubsession::createNew(UsageEnvironment& env,
                                                     char const* fileName,
                                                     Boolean reuseFirstSource) {
    return new ADTSBackChannelAudioFileServerMediaSubsession(env, fileName, reuseFirstSource);
}

ADTSBackChannelAudioFileServerMediaSubsession::ADTSBackChannelAudioFileServerMediaSubsession(UsageEnvironment& env,
                                                char const* fileName, Boolean reuseFirstSource)
: FileServerMediaSubsession(env, fileName, reuseFirstSource),
fAuxSDPLine(NULL),fDoneFlag(0), fRTPTimestampFrequency(8000){
    
}


ADTSBackChannelAudioFileServerMediaSubsession
::~ADTSBackChannelAudioFileServerMediaSubsession() {
}


FileSink* ADTSBackChannelAudioFileServerMediaSubsession::createNewStreamDestination(unsigned clientSessionId,
                                                                          unsigned& estBitrate)
{
    estBitrate = 8; // kbps, estimate
    
    //printf("ADTSBackChannelAudioFileServerMediaSubsession %s %d \n", __FUNCTION__ , __LINE__);
    return ADTSAudioFileSink::createNew(envir(), fFileName,  1024, 0);
}


// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSource* ADTSBackChannelAudioFileServerMediaSubsession::createNewRTPSource(Groupsock* rtpGroupsock,
                                                                             unsigned char rtpPayloadTypeIfDynamic,
                                                                             FileSink* outputSink)
{
    printf("ADTSBackChannelAudioFileServerMediaSubsession::createNewRTPSource %d rtpPayloadTypeIfDynamic:%d\n",__LINE__ ,rtpPayloadTypeIfDynamic);
    //a=fmtp: %d ;profile-level-id=15;mode=AAC-hbr;config=1588;sizeLength=13;indexLength=3;indexDeltaLength=3;profile=1;bitrate=12000
    fRTPPayloadFormat = rtpPayloadTypeIfDynamic;
    fReadSource =
        MPEG4GenericRTPSource::createNew(envir(), rtpGroupsock,
                                       rtpPayloadTypeIfDynamic,
                                       fRTPTimestampFrequency,
                                       "audio",
                                       "aac-hbr",
                                       13,
                                       3,
                                       3
                                       );
    return fReadSource;
}

char const* ADTSBackChannelAudioFileServerMediaSubsession::getAuxSDPLineForBackChannel(FileSink* mediaSink, RTPSource* rtpSource)
{
    if(fAuxSDPLine != NULL)  return fAuxSDPLine;
    
    // TODO: generate fAuxSDPLine
    char pTmpStr[100] = {0};
    sprintf(pTmpStr, "a=fmtp:%d streamtype=5;\r\n", fRTPPayloadFormat);
    fAuxSDPLine = strDup(pTmpStr);
    
    printf("ADTSBackChannelAudioFileServerMediaSubsession::getAuxSDPLine() line:%d, pTmpStr=%s\n", __LINE__, pTmpStr);
    //envir().taskScheduler().doEventLoop(&fDoneFlag);
    return fAuxSDPLine;
}

// implment virtual function defined in OnDemandServerMediaSubsession.hh
// new virtual functions, defined by all subclasses
FramedSource* ADTSBackChannelAudioFileServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
                                            unsigned& estBitrate)
{ return NULL;}

// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSink* ADTSBackChannelAudioFileServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
                                  unsigned char rtpPayloadTypeIfDynamic,
                                  FramedSource* inputSource)
{ return NULL;}




