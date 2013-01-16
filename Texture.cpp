#include "Texture.h"
/*
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

		unsigned char * imageData = new unsigned char[ size ];

		fread( imageData, 1, size, file );

		for( uint cswap = 0; cswap < size; cswap += ( bitsPerPixel / 8 ))
		{
			imageData[cswap] ^= imageData[cswap + 2] ^= imageData[cswap] ^= imageData[cswap + 2];
		};

		fclose( file );

		glGenTextures( 1, &id );

		glBindTexture( GL_TEXTURE_2D, id );

		//glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );
		gluBuild2DMipmaps( GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, imageData );

		glBindTexture( GL_TEXTURE_2D, 0 );		

		delete [] imageData;
	}
	*/
void Texture::_loadTGA( const char * filename, byte mr, byte mg, byte mb )
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

		format = GL_RGBA;

		uint size = width * height * sizeof( int );

		unsigned char * imageData = new unsigned char[ size ];

		if( bitsPerPixel == 24 )
		{
			for( uint i = 0; i < size; i+=4 ) // RGBA
			{			
				byte r, g, b;

				fread( &b, 1, sizeof( r ), file );
				fread( &g, 1, sizeof( b ), file );
				fread( &r, 1, sizeof( g ), file );

				imageData[ i + 0 ] = r; imageData[ i + 1 ] = g; imageData[ i + 2 ] = b; 

				
				if( r == mr && g == mg && b == mb )
					imageData[ i + 3 ] = 0;	
				else
					imageData[ i + 3 ] = 255; // alpha

			}	
		}

		if( bitsPerPixel == 32 )
		{
			for( uint i = 0; i < size; i+=4 ) // RGBA
			{			
				byte r, g, b, a;
								
				fread( &b, 1, sizeof( r ), file );
				fread( &g, 1, sizeof( b ), file );
				fread( &r, 1, sizeof( g ), file );				
				fread( &a, 1, sizeof( a ), file );

				imageData[ i + 0 ] = r; imageData[ i + 1 ] = g; imageData[ i + 2 ] = b; imageData[ i + 3 ] = a;
			}	
		}

		fclose( file );

		glGenTextures( 1, &id );

		glBindTexture( GL_TEXTURE_2D, id );

		//glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );
		gluBuild2DMipmaps( GL_TEXTURE_2D, format, width, height, format, GL_UNSIGNED_BYTE, imageData );

		glBindTexture( GL_TEXTURE_2D, 0 );		

		delete [] imageData;
	}
};

Texture * Texture::loadTGA( const char * filename, byte mr, byte mg, byte mb )
{
	auto texIt = textures.find( string( filename ));

	if( texIt != textures.end() )
		return texIt->second;
	else
	{
		Texture * tex = new Texture();

		tex->_loadTGA( filename , mr, mg, mb );

		textures[ string( filename ) ] = tex;

		return tex;
	};
};

map<string,Texture*> Texture::textures;

void Texture::bind()
{
	glBindTexture( GL_TEXTURE_2D, id );

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
};