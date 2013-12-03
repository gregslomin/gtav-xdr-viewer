#pragma once
#include <iostream>
#include <fstream>
using namespace std;
class ByteStream: public ifstream
{
public:
	ByteStream(void);
	~ByteStream(void);
	unsigned short getu16();
	unsigned int getu32();
	float getfloat();
};

