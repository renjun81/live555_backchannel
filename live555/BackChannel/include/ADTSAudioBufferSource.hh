
#ifndef _ADTS_AUDIO_BUFFER_SOURCE_HH
#define _ADTS_AUDIO_BUFFER_SOURCE_HH

#ifndef _FRAMED_SOURCE_HH
#include "FramedSource.hh"
#endif

class ADTSAudioBufferSource: public FramedSource {
public:
    static ADTSAudioBufferSource* createNew(UsageEnvironment& env, unsigned int vProfile, unsigned int vSmaplingFrequenceIndex, unsigned int vChannels);
   
   unsigned samplingFrequency() const { return fSamplingFrequency; }
   unsigned numChannels() const { return fNumChannels; }
   char const* configStr() const { return fConfigStr; }
   // returns the 'AudioSpecificConfig' for this stream (in ASCII form)
   
   private:
   ADTSAudioBufferSource(UsageEnvironment& env, u_int8_t profile,
   	      u_int8_t samplingFrequencyIndex, u_int8_t channelConfiguration);
   // called only by createNew()
   
   virtual ~ADTSAudioBufferSource();
   
   private:
   // redefined virtual functions:
   virtual void doGetNextFrame();
   
   private:
   unsigned fSamplingFrequency;
   unsigned fNumChannels;
   unsigned fuSecsPerFrame;
   char fConfigStr[5];
};

#endif
