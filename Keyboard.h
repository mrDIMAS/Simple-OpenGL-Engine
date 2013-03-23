#pragma once

#include "Common.h"

class Keyboard
{
	bool keyLast[ 255 ];
public:
	Keyboard( );
	bool keyPressed( uint8_t key );
	bool isLShiftPressed( );

	bool keyHit( uint8_t key )
	{
		return keyPressed( key ) & ( ~keyLast[ key ] );
	};

	void update( )
	{
		for( int i = 0; i < 255; i++ )
			keyLast[ i ] = keyPressed( i );
	};
};

