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
	int lmbs;
	int rmbs;

	Mouse( )
	{
		sx = sy = 0;
		lmbs = 0;
		rmbs = 0;
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

		lmbs = SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_LEFT );
		rmbs = SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_RIGHT );

		saveCurrentCoord();
	};

	bool isLeftButtonPressed( )
	{
		return SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_LEFT );
	};

	bool isRightButtonPressed( )
	{
		return SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_RIGHT );
	};

	bool lmbHit( )
	{
		return SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_LEFT ) & ( ~lmbs );
	};
	bool rmbHit( )
	{
		return SDL_GetMouseState( 0, 0 ) & SDL_BUTTON( SDL_BUTTON_RIGHT ) & ( ~rmbs );
	};
	int getXSpeed()
	{
		return sx;
	};

	int getYSpeed()
	{
		return sy;
	};

	void flush()
	{
		sy = 0;
		sx = 0;
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
};