//
//  WaveBackChannelAudioFileServerMediaSubsession.cpp
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#include "WaveBackChannelAudioFileServerMediaSubsession.hh"

WaveBackChannelAudioFileServerMediaSubsession*
WaveBackChannelAudioFileServerMediaSubsession::createNew(UsageEnvironment& env,
                                                     char const* fileName,
                                                     Boolean reuseFirstSource) {
    return new WaveBackChannelAudioFileServerMediaSubsession(env, fileName, reuseFirstSource);
}

WaveBackChannelAudioFileServerMediaSubsession::WaveBackChannelAudioFileServerMediaSubsession(UsageEnvironment& env,
                                                char const* fileName, Boolean reuseFirstSource)
: FileServerMediaSubsession(env, fileName, reuseFirstSource){
    
}


WaveBackChannelAudioFileServerMediaSubsession
::~WaveBackChannelAudioFileServerMediaSubsession() {
}


FileSink* WaveBackChannelAudioFileServerMediaSubsession::createNewStreamDestination(unsigned clientSessionId,
                                                                          unsigned& estBitrate)
{
    estBitrate = 8; // kbps, estimate
    return FileSink::createNew(envir(), fFileName);
}


// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSource* WaveBackChannelAudioFileServerMediaSubsession::createNewRTPSource(Groupsock* rtpGroupsock,
                                                                             unsigned char rtpPayloadTypeIfDynamic,
                                                                             FileSink* outputSink)
{
	rtpPayloadTypeIfDynamic = 0;
    unsigned fSamplingFrequency = 8000;

    RTPSource* fReadSource = WaveRTPSource::createNew(envir(), rtpGroupsock,
    		rtpPayloadTypeIfDynamic, fSamplingFrequency);

    return fReadSource;
}

// implment virtual function defined in OnDemandServerMediaSubsession.hh
// new virtual functions, defined by all subclasses
FramedSource* WaveBackChannelAudioFileServerMediaSubsession::createNewStreamSource(unsigned clientSessionId,
                                            unsigned& estBitrate)
{ return NULL;}

// "estBitrate" is the stream's estimated bitrate, in kbps
RTPSink* WaveBackChannelAudioFileServerMediaSubsession::createNewRTPSink(Groupsock* rtpGroupsock,
                                  unsigned char rtpPayloadTypeIfDynamic,
                                  FramedSource* inputSource)
{ return NULL;}










WaveRTPSource* WaveRTPSource::createNew(UsageEnvironment& env,
			      Groupsock* RTPgs,
			      unsigned char rtpPayloadFormat,
			      unsigned rtpTimestampFrequency) {
  return new WaveRTPSource(env, RTPgs, rtpPayloadFormat,
				rtpTimestampFrequency);
}

WaveRTPSource::WaveRTPSource(UsageEnvironment& env,
				       Groupsock* rtpGS,
				       unsigned char rtpPayloadFormat,
				       unsigned rtpTimestampFrequency)
  : MultiFramedRTPSource(env, rtpGS,
			 rtpPayloadFormat, rtpTimestampFrequency) {
}

WaveRTPSource::~WaveRTPSource() {
}

char const* WaveRTPSource::MIMEtype() const {
  return "audio";
}



