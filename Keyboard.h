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

	bool isLShiftPressed( )
	{
		SDLMod mod = SDL_GetModState();

		return mod & KMOD_LSHIFT;
	};
} keyboard;