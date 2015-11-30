//
//  AudioUtil.c
//  live555_trace
//
//  Created by albert on 2014/7/3.
//  Copyright (c) 2014年 albert. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "AudioUtility.h"

int parseAACADTSString(uint8_t * pInput, tAACADTSHeaderInfo* pADTSHeader)
{
    int bHasSyncword = FALSE;
    if(pADTSHeader==NULL)
        return FALSE;
    
    // == adts_fixed_header ==
    //   syncword; 12 bslbf should be 0x1111 1111 1111
    if(pInput[0]==0xFF)
    {
        if((pInput[1]&0xF0)==0xF0)
        {
            bHasSyncword = TRUE;
        }
    }
    
    if(!bHasSyncword) return FALSE;
    
    //== adts_fixed_header ==
    //    int   syncword;                // 12 bslbf
    //    int    ID;                       // 1 bslbf
    //    int    layer;                    // 2 uimsbf
    //    int    protection_absent;        // 1 bslbf
    //    int    profile;                  // 2 uimsbf
    //    int    sampling_frequency_index; // 4 uimsbf
    //    int    private_bit;              // 1 bslbf
    //    int    channel_configuration;    // 3 uimsbf
    //    int    original_copy;            // 1 bslbf
    //    int    home;                     // 1 bslbf
    
    pADTSHeader->syncword = 0x0fff;
    pADTSHeader->ID = (pInput[1]&0x08)>>3;
    pADTSHeader->layer = (pInput[1]&0x06)>>2;
    pADTSHeader->protection_absent = pInput[1]&0x01;
    
    pADTSHeader->profile = (pInput[2]&0xC0)>>6;
    pADTSHeader->sampling_frequency_index = (pInput[2]&0x3C)>>2;
    pADTSHeader->private_bit = (pInput[2]&0x02)>>1;
    
    pADTSHeader->channel_configuration = ((pInput[2]&0x01)<<2) + ((pInput[3]&0xC0)>>6);
    pADTSHeader->original_copy = ((pInput[3]&0x20)>>5);
    pADTSHeader->home = ((pInput[3]&0x10)>>4);
    
    
    // == adts_variable_header ==
    //    copyright_identification_bit; 1 bslbf
    //    copyright_identification_start; 1 bslbf
    //    frame_length; 13 bslbf
    //    adts_buffer_fullness; 11 bslbf
    //    number_of_raw_data_blocks_in_frame; 2 uimsfb
    
    pADTSHeader->copyright_identification_bit = ((pInput[3]&0x08)>>3);
    pADTSHeader->copyright_identification_start = ((pInput[3]&0x04)>>2);
    pADTSHeader->frame_length = ((pInput[3]&0x03)<<11) + ((pInput[4])<<3) + ((pInput[5]&0xE0)>>5);
    pADTSHeader->adts_buffer_fullness = ((pInput[5]&0x1F)<<6) + ((pInput[6]&0xFC)>>2);
    pADTSHeader->number_of_raw_data_blocks_in_frame = ((pInput[6]&0x03));
    
    
    // We can't use bits mask to convert byte array to ADTS structure.
    // http://mjfrazer.org/mjfrazer/bitfields/
    // Big endian machines pack bitfields from most significant byte to least.
    // Little endian machines pack bitfields from least significant byte to most.
    // Direct bits mapping is hard....  we should implement a parser ourself.
    
    return TRUE;
    
    
    ;
}


int generateAACADTSString(uint8_t* pBufOut, tAACADTSHeaderInfo* pADTSHeader)
{
    uint8_t pOutput[10]={0};
    if((pADTSHeader==NULL)||(pBufOut==NULL))
    {
        printf("generateAACADTSString error");
        return FALSE;
    }
    
    
    // adts_fixed_header
    //    syncword; 12 bslbf
    //    ID; 1 bslbf
    //    layer; 2 uimsbf
    //    protection_absent; 1 bslbf
    //    profile; 2 uimsbf
    //    sampling_frequency_index; 4 uimsbf
    //    private_bit; 1 bslbf
    //    channel_configuration; 3 uimsbf
    //    original/copy; 1 bslbf
    //    home; 1 bslbf
    
    // adts_variable_header
    //    copyright_identification_bit; 1 bslbf
    //    copyright_identification_start; 1 bslbf
    //    frame_length; 13 bslbf
    //    adts_buffer_fullness; 11 bslbf
    //    number_of_raw_data_blocks_in_frame; 2 uimsfb
    
    
    // == adts_fixed_header ==
    //   syncword; 12 bslbf should be 0x1111 1111 1111
    pOutput[0] = 0xFF;
    pOutput[1] = 0xF0 |
    ((pADTSHeader->ID)<<3) |
    ((pADTSHeader->layer)<<2) |
    ((pADTSHeader->protection_absent));
    
    
    pOutput[2] = ((pADTSHeader->profile)<<6) |
    ((pADTSHeader->sampling_frequency_index)<<2) |
    ((pADTSHeader->private_bit)<<1) |
    ((pADTSHeader->channel_configuration)>>7);
    
    pOutput[3] = ((pADTSHeader->channel_configuration)<<6) |
    ((pADTSHeader->original_copy)<<5) |
    ((pADTSHeader->home)<<4) |
    ((pADTSHeader->copyright_identification_bit)<<3) |
    ((pADTSHeader->copyright_identification_start)<<2) |
    ((pADTSHeader->frame_length>>11)&0x03);
    
    // == adts_variable_header ==
    //    copyright_identification_bit; 1 bslbf
    //    copyright_identification_start; 1 bslbf
    //    frame_length; 13 bslbf
    //    adts_buffer_fullness; 11 bslbf
    //    number_of_raw_data_blocks_in_frame; 2 uimsfb
    
    pOutput[4] = ((pADTSHeader->frame_length)>>3);
    
    pOutput[5] = (((pADTSHeader->frame_length)<<5)&0xE0) |
    ((pADTSHeader->adts_buffer_fullness)>>6);
    
    pOutput[6] = ((pADTSHeader->adts_buffer_fullness)<<2) |
    ((pADTSHeader->number_of_raw_data_blocks_in_frame));
    
    memcpy(pBufOut, pOutput, 7);
    
    return TRUE;
}

// Reference http://wiki.multimedia.cx/index.php?title=ADTS
void printAACAHeader(tAACADTSHeaderInfo *pADTSHeader)
{
    //== adts_fixed_header ==
    //    uint16_t   syncword;                // 12 bslbf
    //    uint8_t    ID;                       // 1 bslbf
    //    uint8_t    layer;                    // 2 uimsbf
    //    uint8_t    protection_absent;        // 1 bslbf
    //    uint8_t    profile;                  // 2 uimsbf
    //    uint8_t    sampling_frequency_index; // 4 uimsbf
    //    uint8_t    private_bit;              // 1 bslbf
    //    uint8_t    channel_configuration;    // 3 uimsbf
    //    uint8_t    original_copy;            // 1 bslbf
    //    uint8_t    home;                     // 1 bslbf
    
    
    // == adts_variable_header ==
    //    copyright_identification_bit; 1 bslbf
    //    copyright_identification_start; 1 bslbf
    //    frame_length; 13 bslbf
    //    adts_buffer_fullness; 11 bslbf
    //    number_of_raw_data_blocks_in_frame; 2 uimsfb
    
    static unsigned const samplingFrequencyTable[16] = {
        96000, 88200, 64000, 48000,
        44100, 32000, 24000, 22050,
        16000, 12000, 11025, 8000,
        7350, 0, 0, 0
    };
    
    printf("== adts_fixed_header ==\n");
    printf("syncword                = 0x%03X\n", pADTSHeader->syncword);
    printf("ID                      = 0x%02X ", pADTSHeader->ID);
    if(pADTSHeader->ID==0) printf("(MPEG-4)");
    else if(pADTSHeader->ID==1) printf("(MPEG-2)");
    else printf("(unknown)");
    printf("\n");
    
    printf("layer                   = 0x%02X\n", pADTSHeader->layer);
    printf("protection_absent       = 0x%02X\n", pADTSHeader->protection_absent);
    printf("profile                 = 0x%02X ", pADTSHeader->profile);
    if(pADTSHeader->profile==0) printf("Main profile (AAC MAIN)");
    else if(pADTSHeader->profile==1) printf("Low Complexity profile (AAC LC)");
    else if(pADTSHeader->profile==2) printf("Scalable Sample Rate profile (AAC SSR)");
    else if(pADTSHeader->profile==3) printf("(reserved) AAC LTP");
    else printf("(unknown)");
    printf("\n");
    
    printf("sampling_frequency_index= 0x%02X (%dHZ)\n", pADTSHeader->sampling_frequency_index, samplingFrequencyTable[pADTSHeader->sampling_frequency_index]);
    printf("private_bit             = 0x%02X\n", pADTSHeader->private_bit);
    printf("channel_configuration   = 0x%02X\n", pADTSHeader->channel_configuration);
    printf("original_copy           = 0x%02X\n", pADTSHeader->original_copy);
    printf("home                    = 0x%02X\n", pADTSHeader->home);
    
    
    
    printf("== adts_variable_header ==\n");
    printf("copyright_identification_bit        = 0x%02X\n", pADTSHeader->copyright_identification_bit);
    printf("copyright_identification_start      = 0x%02X\n", pADTSHeader->copyright_identification_start);
    printf("frame_length                        = 0x%02X\n", pADTSHeader->frame_length);
    printf("adts_buffer_fullness                = 0x%03X\n", pADTSHeader->adts_buffer_fullness);
    printf("number_of_raw_data_blocks_in_frame  = 0x%02X\n", pADTSHeader->number_of_raw_data_blocks_in_frame);
}