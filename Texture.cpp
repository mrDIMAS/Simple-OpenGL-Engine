#include "Texture.h"

void Texture::loadTGA( const char * filename )
{
	TGAHeader targaHeader;
	TGA targa;

	FILE * file = fopen( filename, "rb" );

	fread( &targaHeader, sizeof(TGAHeader), 1, file );

	if( !memcmp( uTGAcompare, &targaHeader, sizeof( targaHeader )))
	{
		fread( targa.Header, sizeof( targa.Header ), 1, file );

		width         = targa.Header[1] * 256 + targa.Header[0];
		height        = targa.Header[3] * 256 + targa.Header[2];
		bitsPerPixel  = targa.Header[4];
	
		format = bitsPerPixel == 24 ? GL_RGB : GL_RGBA;

		uint size = width * height * ( bitsPerPixel / 8 );

		unsigned char * imageData = new unsigned char[ size ];

		fread( imageData, 1, size, file );

		for( uint cswap = 0; cswap < size; cswap += ( bitsPerPixel / 8 ))
		{
			imageData[cswap] ^= imageData[cswap + 2] ^= imageData[cswap] ^= imageData[cswap + 2];
		};

		fclose( file );

		glGenTextures( 1, &id );

		glBindTexture( GL_TEXTURE_2D, id );

		glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );

		glBindTexture( GL_TEXTURE_2D, 0 );

		delete [] imageData;
	}
};

void Texture::loadBMP( const char * filename )
{
	AUX_RGBImageRec * texture1;

	texture1 = auxDIBImageLoadA( filename );

	glGenTextures( 1, &id );

	glBindTexture( GL_TEXTURE_2D, id );

	glTexImage2D( GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data );

	glBindTexture( GL_TEXTURE_2D, 0 );
};

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
};