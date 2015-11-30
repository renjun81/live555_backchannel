//
//  ADTSBackChannelAudioFileServerMediaSubsession.h
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#ifndef __live555_trace__ADTSBackChannelAudioFileServerMediaSubsession__
#define __live555_trace__ADTSBackChannelAudioFileServerMediaSubsession__

#ifndef _FILE_SERVER_MEDIA_SUBSESSION_HH
#include "FileServerMediaSubsession.hh"
#endif

class ADTSBackChannelAudioFileServerMediaSubsession: public FileServerMediaSubsession{

public:
    static ADTSBackChannelAudioFileServerMediaSubsession*
    createNew(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource);
    
protected:
    ADTSBackChannelAudioFileServerMediaSubsession(UsageEnvironment& env,
                                       char const* fileName, Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~ADTSBackChannelAudioFileServerMediaSubsession();
    void setDoneFlag() { fDoneFlag=~0; };
    
protected: // redefined virtual functions

    // Below functions should be redefined,
    // But they won't use in backchannel
    virtual FramedSource* createNewStreamSource(unsigned clientSessionId,
                          unsigned& estBitrate);
    //"estBitrate" is the stream's estimated bitrate, in kbps
    virtual RTPSink* createNewRTPSink(Groupsock* rtpGroupsock,
                    unsigned char rtpPayloadTypeIfDynamic,
                    FramedSource* inputSource);
    
    virtual FileSink* createNewStreamDestination(unsigned clientSessionId,
                                    unsigned& estBitrate);
    // "estBitrate" is the stream's estimated bitrate, in kbps
    virtual RTPSource* createNewRTPSource(Groupsock* rtpGroupsock,
                                          unsigned char rtpPayloadTypeIfDynamic,
                                          FileSink* outputSink);
    
    // SDP for RTP Server with normal streaming
    //virtual char const* getAuxSDPLine(RTPSource *rtpSource, FileSink *outputSink);

    // SDP for RTP Server with backchannel streaming
    virtual char const* getAuxSDPLineForBackChannel(FileSink* mediaSink, RTPSource* rtpSource);
    // Below is for backchannel
    // reference MediaSession.cpp
    
    //virtual Boolean createSourceObjects(int useSpecialRTPoffset);
    
    unsigned char fRTPPayloadFormat;
    unsigned fRTPTimestampFrequency;
    char* fMediumName;
    
    Groupsock* fRTPSocket; Groupsock* fRTCPSocket; // works even for unicast
    RTPSource* fRTPSource; RTCPInstance* fRTCPInstance;
    //FramedSource* fReadSource;
    RTPSource *fReadSource;
    
    // Other fields:
    char* fSessionId; // used by RTSP

private:
    char* fAuxSDPLine;
    char fDoneFlag;
};


#endif /* defined(__live555_trace__ADTSBackChannelAudioFileServerMediaSubsession__) */
