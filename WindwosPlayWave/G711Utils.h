#ifndef G711UTtils_HEADER_H
#define G711UTtils_HEADER_H

#include <stdlib.h>

typedef char int8_t;
typedef unsigned char uint8_t;
typedef short int16_t;
typedef unsigned short uint16_t;

int8_t ALaw_Encode(int16_t number);
int16_t ALaw_Decode(int8_t number);
int8_t MuLaw_Encode(int16_t number);
int16_t MuLaw_Decode(int8_t number);

int16_t* decodeMuLawStream(int8_t* buffer, size_t length);

#endif