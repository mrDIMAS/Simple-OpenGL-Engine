#include "Keyboard.h"

Keyboard::Keyboard( )
{

};

bool Keyboard::keyPressed( uint8_t key )
{
	uint8_t * keyboard = SDL_GetKeyState( 0 );

	return keyboard[ key ];
};

bool Keyboard::isLShiftPressed( )
{
	SDLMod mod = SDL_GetModState();

	return mod & KMOD_LSHIFT;
};

Keyboard keyboard;