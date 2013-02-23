#pragma once

#include "Common.h"

class Keyboard
{
public:
	Keyboard( );
	bool keyPressed( uint8_t key );
	bool isLShiftPressed( );
};

