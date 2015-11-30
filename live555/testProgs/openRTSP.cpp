/**********
This library is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the
Free Software Foundation; either version 2.1 of the License, or (at your
option) any later version. (See <http://www.gnu.org/copyleft/lesser.html>.)

This library is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License
along with this library; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
**********/
// Copyright (c) 1996-2014, Live Networks, Inc.  All rights reserved
// A RTSP client application that opens a RTSP URL argument,
// and extracts and records the data from each incoming RTP stream.
//
// NOTE: If you want to develop your own RTSP client application (or embed RTSP client functionality into your own application),
// then we don't recommend using this code as a model, because it is too complex (with many options).
// Instead, we recommend using the "testRTSPClient" application code as a model.

#include "playCommon.hh"

RTSPClient* ourRTSPClient = NULL;
Medium* createClient(UsageEnvironment& env, char const* url, int verbosityLevel, char const* applicationName) {
  extern portNumBits tunnelOverHTTPPortNum;
  return ourRTSPClient = RTSPClient::createNew(env, url, verbosityLevel, applicationName, tunnelOverHTTPPortNum);
}

void assignClient(Medium* client) {
  ourRTSPClient = (RTSPClient*)client;
}

void getOptions(RTSPClient::responseHandler* afterFunc) { 
  ourRTSPClient->sendOptionsCommand(afterFunc, ourAuthenticator);
}

void getSDPDescription(RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendDescribeCommand(afterFunc, ourAuthenticator);
}

void setupSubsession(MediaSubsession* subsession, Boolean streamUsingTCP, Boolean forceMulticastOnUnspecified, RTSPClient::responseHandler* afterFunc) {
  
  ourRTSPClient->sendSetupCommand(*subsession, afterFunc, False, streamUsingTCP, forceMulticastOnUnspecified, ourAuthenticator);
}

void startPlayingSession(MediaSession* session, double start, double end, float scale, RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendPlayCommand(*session, afterFunc, start, end, scale, ourAuthenticator);
}

void startPlayingSession(MediaSession* session, char const* absStartTime, char const* absEndTime, float scale, RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendPlayCommand(*session, afterFunc, absStartTime, absEndTime, scale, ourAuthenticator);
}

void tearDownSession(MediaSession* session, RTSPClient::responseHandler* afterFunc) {
  ourRTSPClient->sendTeardownCommand(*session, afterFunc, ourAuthenticator);
}

void setUserAgentString(char const* userAgentString) {
  ourRTSPClient->setUserAgentString(userAgentString);
}

Boolean allowProxyServers = False;
Boolean controlConnectionUsesTCP = True;
Boolean supportCodecSelection = False;
char const* clientProtocolName = "RTSP";

// 20140703 albert.liao modified start
#pragma mark - Below is test for backchannel
//#include "ADTSAudioBufferSource.hh"
//#define TEST_WITH_NEW_CLASS_AUDIO_BUFFER_SOURCE 1
#ifdef TEST_WITH_NEW_CLASS_AUDIO_BUFFER_SOURCE
#include "ADTSAudioBufferSource.hh"
#endif

#pragma mark - MediaSubsession virtual function

FramedSource* MediaSubsession
::createNewStreamSource(unsigned /*clientSessionId*/, unsigned& estBitrate) {
    estBitrate = 96; //96; // kbps, estimate
    
#ifdef TEST_WITH_NEW_CLASS_AUDIO_BUFFER_SOURCE
    FramedSource *pTmp = ADTSAudioBufferSource::createNew(fParent.envir(), 1, 11 /*8000*/, 1);
    //ADTSAudioBufferSource* createNew(UsageEnvironment& env, unsigned int vProfile, unsigned int vSmaplingFrequenceIndex, unsigned int vChannels);
#else
    // The test file is get from http://www.live555.com/liveMedia/public/aac/
    //FramedSource *pTmp = ADTSAudioFileSource::createNew(fParent.envir(), "/Users/liaokuohsun/Work/AudioTestSample/test.aac");
    FramedSource *pTmp = ADTSAudioFileSource::createNew(fParent.envir(), "/Users/liaokuohsun/Downloads/1024.aac");
#endif
    return pTmp;
}

RTPSink* MediaSubsession
::createNewRTPSink(Groupsock* rtpGroupsock,
                   unsigned char rtpPayloadTypeIfDynamic,
                   FramedSource* inputSource) {
    
#ifdef TEST_WITH_NEW_CLASS_AUDIO_BUFFER_SOURCE
    ADTSAudioBufferSource* adtsSource = (ADTSAudioBufferSource*)inputSource;
#else
    ADTSAudioFileSource* adtsSource = (ADTSAudioFileSource*)inputSource;
#endif
    
    RTPSink *pTmp = MPEG4GenericRTPSink::createNew(fParent.envir(), rtpGroupsock,
                                                   rtpPayloadTypeIfDynamic,
                                                   adtsSource->samplingFrequency(),
                                                   "audio", "aac-hbr", adtsSource->configStr(),
                                                   adtsSource->numChannels());
    return pTmp;
}

Boolean MediaSubsession::createSinkObjects(int useSpecialRTPoffset)
{
    do {
        // TODO: Fix me
        clientSessionId = 0;
        streamBitrate = 12000;
        
        fReadSource = createNewStreamSource(clientSessionId, streamBitrate);
        if(!fReadSource)
        {
            fprintf(stderr, "MediaSubsession::createSinkObjects()  fReadSource==NULL\n");
            break;
        }
        
        fRTPSink = createNewRTPSink(fRTPSocket, 96, fReadSource);
        
        if(!fRTPSink)
        {
            fprintf(stderr, "MediaSubsession::createSinkObjects()  fRTPSink==NULL\n");
            break;
        }
        
        return True;
    } while (0);
    
    return False; // an error occurred
}
// 20140703 albert.liao modified end
