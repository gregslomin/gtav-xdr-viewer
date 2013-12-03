#pragma once
#include <iostream>
#include <stdio.h>
using namespace std;
class ByteStream
{
private:
	FILE* fp;
public:
	void open(char* FilePath);
	int is_open();
	FILE* getfp();
	ByteStream(void);
	~ByteStream(void);
	void seekg(int offset);
	unsigned short getu16();
	unsigned int getu32();
	float getfloat();
	void read(void* dst, int size);
};

