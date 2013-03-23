#include "Texture.h"

#define TEXBUF_SIZE 1024

unsigned char textureBuffer[ TEXBUF_SIZE * TEXBUF_SIZE * 4 ] = { 0 };
unsigned char resizedTextureBuffer[ TEXBUF_SIZE * TEXBUF_SIZE * 4 ] = { 0 };

int getNearestPow2( int num )
{
	if( num >= 1024 )
		return 1024;
	if( num >= 768 && num < 1024 )
		return 1024;
	if( num >= 512 && num < 768 )
		return 512;
	if( num >= 384 && num < 512 )
		return 512;
	if( num >= 256 && num < 384 )
		return 256;
	if( num >= 192 && num < 256 )
		return 256;
	if( num >= 128 && num < 192 )
		return 128;
	if( num >= 96 && num < 128 )
		return 128;
	if( num >= 64 && num < 96 )
		return 64;
	if( num >= 48 && num < 64 )
		return 64;
	if( num >= 32 && num < 48 )
		return 32;
	if( num >= 24 && num < 32 )
		return 32;
	if( num >= 16 && num < 24 )
		return 16;

	return 8;
};

void Texture::_loadTGA( const char * filename, bool useMips )
{
	TGAHeader targaHeader;
	TGA targa;

	FILE * file = fopen( filename, "rb" );

	if( !file )
		return;

	fread( &targaHeader, sizeof(TGAHeader), 1, file );

	if( !memcmp( uTGAcompare, &targaHeader, sizeof( targaHeader )))
	{
		fread( targa.Header, sizeof( targa.Header ), 1, file );

		width         = targa.Header[1] * 256 + targa.Header[0];
		height        = targa.Header[3] * 256 + targa.Header[2];
		bitsPerPixel  = targa.Header[4];

		format = bitsPerPixel == 24 ? GL_RGB : GL_RGBA;

		uint size = width * height * ( bitsPerPixel / 8 );

		fread( textureBuffer, 1, size, file );

		for( uint cswap = 0; cswap < size; cswap += ( bitsPerPixel / 8 ))
		{
			textureBuffer[cswap] ^= textureBuffer[cswap + 2] ^= textureBuffer[cswap] ^= textureBuffer[cswap + 2];
		};

		fclose( file );

		glGenTextures( 1, &id );

		glBindTexture( GL_TEXTURE_2D, id );
		
		float scaler = 1;

		switch( loadQuality )
		{
		case QUALITY_BEST:
			scaler = 1;
			break;
		case QUALITY_HIGH:
			scaler = 0.75;
			break;
		case QUALITY_MEDIUM:
			scaler = 0.5;
			break;
		case QUALITY_LOW:
			scaler = 0.25;
			break;
		case QUALITY_EXTRA_LOW:
			scaler = 0.15;
			break;
		};
		
		int newW = getNearestPow2( width * scaler );
		int newH = getNearestPow2( height * scaler );
	
		gluScaleImage( format, width, height, GL_UNSIGNED_BYTE, textureBuffer, newW, newH, GL_UNSIGNED_BYTE, resizedTextureBuffer );
		
		if( useMips )
		{
			gluBuild2DMipmaps( GL_TEXTURE_2D, format, newW, newH, format, GL_UNSIGNED_BYTE, resizedTextureBuffer );

			filter = GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			glTexImage2D( GL_TEXTURE_2D, 0, format, newW, newH, 0, format, GL_UNSIGNED_BYTE, resizedTextureBuffer );

			filter = GL_LINEAR;
		}

		glBindTexture( GL_TEXTURE_2D, 0 );		
	}
};

Texture * Texture::loadTGA( const char * filename, bool useMips )
{
	auto texIt = textures.find( string( filename ));

	if( texIt != textures.end() )
		return texIt->second;
	else
	{
		Texture * tex = new Texture();

		tex->_loadTGA( filename, useMips );

		textures[ string( filename ) ] = tex;

		return tex;
	};
};

map<string,Texture*> Texture::textures;
int Texture::loadQuality = Texture::QUALITY_BEST;
void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
};