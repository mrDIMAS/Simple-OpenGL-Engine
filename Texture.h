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
	void _loadTGA( const char * filename, bool useMips );

	Texture()
	{
	}
public:
	enum 
	{
		QUALITY_BEST,
		QUALITY_HIGH,
		QUALITY_MEDIUM,
		QUALITY_LOW,
		QUALITY_EXTRA_LOW,
	};

	uint id;
	uint width;
	uint height;
	uint bitsPerPixel;
	uint format;
	uint filter;

	static map<string,Texture*> textures;

	static int loadQuality;

	static Texture * loadTGA( const char * filename, bool useMips = false );

	static void deleteAll()
	{
		for( auto it = textures.begin(); it != textures.end(); it++ )
			delete it->second;
	};

	void bind();
};