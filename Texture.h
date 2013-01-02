#pragma once

#include "Common.h"

static unsigned char uTGAcompare[12] = {0,0,2,0,0,0,0,0,0,0,0};

struct TGAHeader
{
	unsigned char Header[12];
};

struct TGA
{
	unsigned char Header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;
	unsigned int type;
	unsigned int Height;
	unsigned int Width;
	unsigned int BPP;
};

class Texture
{
public:
	uint id;
	uint width;
	uint height;
	uint bitsPerPixel;
	uint format;

	void loadTGA( const char * filename );

	void loadBMP( const char * filename );

	void bind();
};