#include "Texture.h"

#define TEXBUF_SIZE 1024

unsigned char textureBuffer[ TEXBUF_SIZE * TEXBUF_SIZE * 4 ];

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

		if( useMips )
		{
			gluBuild2DMipmaps( GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, textureBuffer );

			filter = GL_LINEAR_MIPMAP_LINEAR;
		}
		else
		{
			glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureBuffer );

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

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter );
};