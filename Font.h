#pragma once

#include "Common.h"

class Font 
{
private:
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
	uint texture;
	position table[ 256 ];
	int cellSize;
	int texSize;
public:
	Font( const char * name, bool italic, bool underline );
	int getRasterCellSize( );
	~Font( );
	void drawFull( );
	void drawSymbol( int x, int y, int c, float scalerx,float scalery );
};