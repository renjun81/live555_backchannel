//
//  WaveBackChannelAudioFileServerMediaSubsession.h
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#ifndef __live555_trace__WaveBackChannelAudioFileServerMediaSubsession__
#define __live555_trace__WaveBackChannelAudioFileServerMediaSubsession__

#include "FileServerMediaSubsession.hh"
#include "MultiFramedRTPSource.hh"

class WaveBackChannelAudioFileServerMediaSubsession: public FileServerMediaSubsession{

public:
    static WaveBackChannelAudioFileServerMediaSubsession*
    createNew(UsageEnvironment& env, char const* fileName, Boolean reuseFirstSource);
    
protected:
    WaveBackChannelAudioFileServerMediaSubsession(UsageEnvironment& env,
                                       char const* fileName, Boolean reuseFirstSource);
    // called only by createNew();
    virtual ~WaveBackChannelAudioFileServerMediaSubsession();
    
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
};

class WaveRTPSource: public MultiFramedRTPSource {
public:
  static WaveRTPSource*
  createNew(UsageEnvironment& env, Groupsock* RTPgs,
	    unsigned char rtpPayloadFormat,
	    unsigned rtpTimestampFrequency);

protected:
  virtual ~WaveRTPSource();

private:
  WaveRTPSource(UsageEnvironment& env, Groupsock* RTPgs,
		     unsigned char rtpPayloadFormat,
		     unsigned rtpTimestampFrequency);
      // called only by createNew()

private:
  virtual char const* MIMEtype() const;
};

#endif /* defined(__live555_trace__WaveBackChannelAudioFileServerMediaSubsession__) */
