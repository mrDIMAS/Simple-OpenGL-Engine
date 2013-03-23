#include "Font.h"

Font::Font( const char * name, bool italic, bool underline )
{
	texSize = 512;

	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	glGenTextures( 1, &texture );

	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	HDC dc = CreateCompatibleDC( 0 );

	cellSize = 32;

	HFONT font = CreateFontA( cellSize, 0, 0, 0, FW_BOLD, italic, underline, 0, RUSSIAN_CHARSET, OUT_TT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE | DEFAULT_PITCH, name );

	SelectObject( dc, font );

	uint listBase = glGenLists( 256 );

	bool result = wglUseFontBitmapsA( dc, 0, 256, listBase );
		
	glViewport( 0, 0, texSize, texSize );

	glMatrixMode( GL_PROJECTION ); glPushMatrix( ); glLoadIdentity(); gluOrtho2D( 0, texSize, 0, texSize );		
	glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();	

	glClearColor( 1, 1, 1, 1 );
	glColor4f( 1, 1, 1, 1 );

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
	float abs = cellSize / (float)texSize;

	for( int i = 0; i < 16; i++ )
	{
		for( int j = 0; j < 16; j++ )
		{
			int index = i * 16 + j;
			table[ index ] = position( j * abs, i * abs );
			glRasterPos2i( j * cellSize, i * cellSize + cellSize / 4 );
			glCallList( index + 1 );
		}
	}	

	glCopyTexImage2D( GL_TEXTURE_2D, 0 , GL_RGB, 0, 0, texSize, texSize, 0 );

	byte * pixels = new byte[ texSize * texSize * 3 ];	glGetTexImage( GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels );
	byte * newPixels = new byte[ texSize * texSize * 4 ];

	uint n = 0;

	for( uint i = 0; i < texSize * texSize * 4; i+=4 )
	{
		newPixels[ i + 0 ] = 255;
		newPixels[ i + 1 ] = 255;
		newPixels[ i + 2 ] = 255;
		newPixels[ i + 3 ] = ~pixels[ n ];

		n+=3;
	};
	delete [] pixels;
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, texSize, texSize, 0, GL_RGBA, GL_UNSIGNED_BYTE, newPixels );
	delete [] newPixels;

	glViewport( 0, 0, info->current_w, info->current_h );

	glPopMatrix();
	glPopMatrix();
		
	glBindTexture( GL_TEXTURE_2D, 0 );

	glDeleteLists( listBase, 255 );

	DeleteDC( dc );
};

int Font::getRasterCellSize( )
{
	return cellSize;
};

Font::~Font( )
{
	glDeleteTextures( 1, &texture );
};

void Font::drawFull( )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	// set ortho projection
	glMatrixMode( GL_PROJECTION );	glPushMatrix(); glLoadIdentity();
	gluOrtho2D( 0, info->current_w, 0, info->current_h );

	glPushAttrib( GL_LIGHTING );		
	glPushAttrib( GL_CULL_FACE );

	glDisable( GL_LIGHTING );
	glDisable( GL_CULL_FACE );

	// set identity view matrix
	glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();

	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glBegin( GL_QUADS );

		glTexCoord2f( 0, 0 );
		glVertex3f(   0, 0, 0 );

		glTexCoord2f( 1, 0 );
		glVertex3f(   texSize, 0, 0 );

		glTexCoord2f( 1, 1 );
		glVertex3f(   texSize, texSize, 0 );

		glTexCoord2f( 0, 1 );
		glVertex3f(   0, texSize, 0 );
	glEnd();

	glPopAttrib();
	glPopAttrib();

	glMatrixMode( GL_MODELVIEW );
	glPopMatrix();
	glMatrixMode( GL_PROJECTION );
	glPopMatrix();

	glMatrixMode( GL_MODELVIEW );
};

void Font::drawSymbol( int x, int y, int c, float scalerx,float scalery )
{
	float tx = table[ c ].x;
	float ty = table[ c ].y;

	float tw = 0.0625;
	float th = 0.0625;

	float px = x;
	float py = y;

	float pw = cellSize * scalerx;
	float ph = cellSize * scalery;

	glBindTexture( GL_TEXTURE_2D, texture );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

	glBegin( GL_QUADS );	
		
		glTexCoord2f( tx, ty + th );
		glVertex3f(   px, py, 0 );

		glTexCoord2f( tx + tw, ty + th );
		glVertex3f(   px + pw, py, 0 );

		glTexCoord2f( tx + tw, ty );
		glVertex3f(   px + pw, py + ph, 0 );

		glTexCoord2f( tx, ty );
		glVertex3f(   px, py + ph, 0 );
	glEnd();
};