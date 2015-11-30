//
//  AudioUtility.h
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#ifndef live555_trace_AudioUtility_h
#define live555_trace_AudioUtility_h

// Reference : https://github.com/mstorsjo/libav/blob/fdk-aac/libavcodec/aacadtsdec.c
#define TRUE  1
#define FALSE 0
typedef unsigned short uint16_t;
typedef unsigned char uint8_t;
typedef struct AACADTSHeaderInfo {
    
    // == adts_fixed_header ==
    uint16_t   syncword;                // 12 bslbf
    uint8_t    ID;                       // 1 bslbf
    uint8_t    layer;                    // 2 uimsbf
    uint8_t    protection_absent;        // 1 bslbf
    uint8_t    profile;                  // 2 uimsbf
    uint8_t    sampling_frequency_index; // 4 uimsbf
    uint8_t    private_bit;              // 1 bslbf
    uint8_t    channel_configuration;    // 3 uimsbf
    uint8_t    original_copy;            // 1 bslbf
    uint8_t    home;                     // 1 bslbf
    
    // == adts_variable_header ==
    uint8_t copyright_identification_bit; //1 bslbf
    uint8_t copyright_identification_start; //1 bslbf
    uint16_t frame_length; //13 bslbf
    uint16_t adts_buffer_fullness; //11 bslbf
    uint8_t number_of_raw_data_blocks_in_frame; //2 uimsfb
    
} tAACADTSHeaderInfo;

extern int parseAACADTSString(uint8_t * pInput, tAACADTSHeaderInfo* pADTSHeader);
extern int generateAACADTSString(uint8_t* pBufOut, tAACADTSHeaderInfo* pADTSHeader);
extern void printAACAHeader(tAACADTSHeaderInfo *pADTSHeader);


#endif
