#pragma once

#include "Common.h"
#include <Windows.h>
#include "Mouse.h"
struct position
{
	float x, y;

	position( float _x, float _y )
	{
		x=_x;y=_y;
	};
	position()
	{
		x=0;y=0;
	};
};

class Font 
{
	uint texture;
	position table[ 256 ];
	int cellSize;
	int texSize;
public:
	Font( const char * name, bool italic, bool underline )
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

		uint listBase = glGenLists( 255 );

		bool result = wglUseFontBitmapsA( dc, 0, 255, listBase );
		
		glViewport( 0, 0, texSize, texSize );

		glMatrixMode( GL_PROJECTION ); glPushMatrix( ); glLoadIdentity(); gluOrtho2D( 0, texSize, texSize, 0 );
		glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();		

		glClearColor( 1, 1, 1, 1 );
		glColor4f( 1, 1, 1, 1 );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		
		for( uint i = 0; i < 16; i++ )
		{
			for( uint j = 0; j < 16; j++ )
			{
				table[ i * 16 + j ] = position( ( j * cellSize ) / (float)texSize, ( i * cellSize ) / (float)texSize );
				glRasterPos2i( j * cellSize, cellSize - cellSize / 4 + i * cellSize);
				glCallList( ( i * 16 + j ) );
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

	int getRasterCellSize( )
	{
		return cellSize;
	};

	~Font( )
	{
		glDeleteTextures( 1, &texture );
	};

	void drawSymbol( int x, int y, int c, float scaler )
	{
		float tx = table[ c ].x;
		float ty = table[ c ].y;

		float tw = 0.0625;
		float th = 0.0625;

		float px = x;
		float py = y;

		float pw = cellSize * scaler;
		float ph = cellSize * scaler;

		glBindTexture( GL_TEXTURE_2D, texture );

		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );

		glBegin( GL_QUADS );	
		
			glTexCoord2f( tx, ty );
			glVertex3f(   px, py, 0 );

			glTexCoord2f( tx + tw, ty );
			glVertex3f(   px + pw, py, 0 );

			glTexCoord2f( tx + tw, ty + th );
			glVertex3f(   px + pw, py + ph, 0 );

			glTexCoord2f( tx, ty + th );
			glVertex3f(   px, py + ph, 0 );

		glEnd();
	};
	/*
	void drawTest()
	{

		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		// set ortho projection
		glMatrixMode( GL_PROJECTION );	glPushMatrix(); glLoadIdentity();
		gluOrtho2D( 0, info->current_w, info->current_h, 0 );

		glPushAttrib( GL_LIGHTING );		
		glPushAttrib( GL_CULL_FACE );

		glDisable( GL_LIGHTING );
		glDisable( GL_CULL_FACE );

		glDisable( GL_ALPHA_TEST );
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
		glEnable( GL_ALPHA_TEST );
		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();

		glMatrixMode( GL_MODELVIEW );
	};*/
};


class Text
{
private:
	Font * font;
	string text;
	int x, y;
	float kerning;
	bool visible;
	float scaler;
	int w,h;
public:
	static vector< Text* > texts;

	Text( Font * font, const char * text, int x, int y )
	{
		this->font = font;		
		this->x	   = x;
		this->y	   = y;
		visible = true;
		kerning = 0.75;
		scaler = 1;
		setText( text );
		texts.push_back( this );
	};

	Text( Font * font )
	{
		this->font = font;
		x = 0;
		y = 0;
		visible = true;
		w=0;
		h=0;
		texts.push_back( this );
	};

	void setSize( float size )
	{
		if( size > getSize() )
		{
			kerning += scaler * 0.0020;
		}
		else
		{
			kerning -= scaler * 0.0013;
		};
		this->scaler = size / font->getRasterCellSize();
		
	};

	float getSize() 
	{
		return scaler * font->getRasterCellSize();
	};

	void setKerning( float k )
	{
		kerning = k;
	};

	void setText( const string & txt )
	{
		text = txt;

		w = txt.size() * getSize();
		h = getSize();
	};

	bool isMouseIn( )
	{
		return mouse.getX() > x && mouse.getX() < ( x + w ) && mouse.getY() > ( y + getSize() )  && mouse.getY() < ( ( y + getSize() ) + h );
	};

	void setPosition( int x, int y )
	{
		this->x = x; this->y = y;
	};

	void hide( )
	{
		visible = false;
	};

	void show()
	{
		visible = true;
	};

	bool isVisible()
	{
		return visible;
	};

	static void renderAll()
	{
		glMatrixMode( GL_TEXTURE ); glPushMatrix(); glLoadIdentity();
		glScalef( 1.0f, -1.0f, 1.0f );

		// render 
		for( uint i = 0; i < texts.size(); i++ )
		{
			texts.at( i )->draw();
		};

		glMatrixMode( GL_TEXTURE );
		glPopMatrix();
	}

	static void deleteAll() 
	{
		// render 
		for( uint i = 0; i < texts.size(); i++ )
		{
			delete texts.at( i );
		};
	};
	
	void draw( )
	{
		if( !visible )
			return;
		
		int ty = y;
		int symbCountOnLine = 0;

		for( uint i = 0; i < text.size(); i++ )
		{	
			int symbol = text[ i ] + 1 ;

			if( symbol == '\n' + 1 )
			{
				ty += scaler * font->getRasterCellSize();

				symbCountOnLine = 0;

				continue;
			}
			
			if( symbol == '\t' + 1 )
			{
				symbCountOnLine += 4;

				continue;
			}

			font->drawSymbol( x + symbCountOnLine * ( scaler * font->getRasterCellSize() * kerning ), ty + scaler * font->getRasterCellSize(), symbol, scaler );

			symbCountOnLine++;
		};
	};
};
