#pragma once

#include "Common.h"

class Mouse
{
private:
	void saveCurrentCoord()
	{
		SDL_GetMouseState( &lx, &ly );
	};
public:
	int lx, ly;
	int sx, sy;

	Mouse( )
	{
		sx = sy = 0;
		saveCurrentCoord();
	};

	void moveTo( ushort x, ushort y )
	{
		SDL_WarpMouse( x, y );

		lx = x; ly = y;
	};

	void update()
	{
		sx = getX() - lx;
		sy = getY() - ly;

		saveCurrentCoord();
	};

	int getXSpeed()
	{
		return sx;
	};

	int getYSpeed()
	{
		return sy;
	};

	int getX()
	{
		int x, y;

		SDL_GetMouseState( &x, &y );

		return x;
	};

	int getY()
	{
		int x, y;

		SDL_GetMouseState( &x, &y );

		return y;
	};
} mouse;