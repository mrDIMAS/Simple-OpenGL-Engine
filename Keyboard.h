#pragma once

#include "Common.h"

class Keyboard
{
public:
	Keyboard( )
	{

	};

	bool keyPressed( uint8_t key )
	{
		uint8_t * keyboard = SDL_GetKeyState( 0 );

		return keyboard[ key ];
	};
} keyboard;