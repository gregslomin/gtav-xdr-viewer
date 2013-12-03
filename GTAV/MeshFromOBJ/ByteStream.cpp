#include "DXUT.h"
#include "ByteStream.h"
#include <stdint.h>
float swap_float32( const float inFloat )
{
   float retVal;
   char *floatToConvert = ( char* ) & inFloat;
   char *returnFloat = ( char* ) & retVal;

   // swap the bytes into a temporary buffer
   returnFloat[0] = floatToConvert[3];
   returnFloat[1] = floatToConvert[2];
   returnFloat[2] = floatToConvert[1];
   returnFloat[3] = floatToConvert[0];

   return retVal;
}
uint16_t swap_uint16( uint16_t val ) 
{
    return (val << 8) | (val >> 8 );
}

//! Byte swap short
int16_t swap_int16( int16_t val ) 
{
    return (val << 8) | ((val >> 8) & 0xFF);
}

//! Byte swap unsigned int
uint32_t swap_uint32( uint32_t val )
{
    val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | (val >> 16);
}

//! Byte swap int
int32_t swap_int32( int32_t val )
{
    val = ((val << 8) & 0xFF00FF00) | ((val >> 8) & 0xFF00FF ); 
    return (val << 16) | ((val >> 16) & 0xFFFF);
}

ByteStream::ByteStream(void)
{
}


ByteStream::~ByteStream(void)
{
}

	unsigned short ByteStream::getu16(){
		unsigned short temp;
		read((char*)&temp, 2);
		return swap_uint16(temp);
	}
	unsigned int ByteStream::getu32(){
	unsigned int temp;
	read((char*)&temp, 4);
	return swap_uint32(temp);
	}
	float ByteStream::getfloat(){
	float temp;
	read((char*)&temp, 4);
	return swap_float32(temp);
	}