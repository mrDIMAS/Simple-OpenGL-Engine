#pragma once

#include "Common.h"
#include "Mouse.h"
#include "Texture.h"

class Image
{
private:
	Texture * tex;
	int x, y, w, h;
	int ox,oy;
	Color clr;
	float scaler;
	void render( )
	{
		if( !visible )
			return;

		tex->bind();
		
		//glColor4f( clr.getRed(), clr.getGreen(), clr.getBlue(), 0.96 );
		//glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

		glBegin( GL_QUADS );		
		
		glTexCoord2f( 0, 1 );
		glVertex3f( x + ox, y + oy, 0 );

		glTexCoord2f( 1, 1 );
		glVertex3f( x + ox + w * scaler, y + oy, 0 );

		glTexCoord2f( 1, 0 );
		glVertex3f( x + w * scaler + ox, y + h * scaler + oy, 0 );

		glTexCoord2f( 0, 0 );
		glVertex3f( x + ox, y + h * scaler + oy, 0 );

		glEnd();	

		//glColor4f( 1, 1, 1, 1 );

		glBindTexture( GL_TEXTURE_2D, 0 );
	};

	bool visible;

public:
	static vector< Image* > images;

	Image( Texture * data ) : clr( 255, 255, 255, 255 )
	{
		tex = data;
		w = data->width;
		h = data->height;
		x = 0;
		y = 0;
		ox = 0;
		oy = 0;
		images.push_back( this );
		scaler = 1;
		visible = true;
	};

	void hide()
	{
		visible = false;
	};

	void show() 
	{
		visible = true;
	};

	int getX()
	{
		return x;
	};

	int getY()
	{
		return y;
	}

	int getWidth()
	{
		return w;
	}

	int getHeight()
	{
		return h;
	}

	void setScaler( float scl )
	{
		scaler = scl;
	};

	float getScaler( )
	{
		return scaler;
	};

	void setOffset( int ox, int oy )
	{
		this->ox = ox; this->oy = oy;
	};

	void setPosition( int x, int y )
	{
		this->x = x; this->y = y;
	};

	void setSize( int w, int h )
	{
		this->w = w; this->h = h;
	};

	void setColor( Color newColor )
	{
		clr = newColor;
	};

	bool isCursorIn( )
	{
		return mouse.getX() >= x && mouse.getX() <= ( x + w * scaler ) && mouse.getY() >= y && ( mouse.getY() ) <= ( y + h * scaler );
	};

	static void renderAll( )
	{
		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		// set ortho projection
		glMatrixMode( GL_PROJECTION );	glPushMatrix(); glLoadIdentity();
		gluOrtho2D( 0, info->current_w, info->current_h, 0 );

		glPushAttrib( GL_LIGHTING );		
		glPushAttrib( GL_CULL_FACE );

		glDisable( GL_LIGHTING );
		glDisable( GL_CULL_FACE );

		// set identity view matrix
		glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();
		
		// render 
		for( uint i = 0; i < images.size(); i++ )
		{
			images.at( i )->render();
		};

		glPopAttrib();
		glPopAttrib();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();

		glMatrixMode( GL_MODELVIEW );
	};

	static void deleteAll()
	{
		for( uint i = 0; i < images.size(); i++ )
		{
			delete images.at( i );
		};
	};
};