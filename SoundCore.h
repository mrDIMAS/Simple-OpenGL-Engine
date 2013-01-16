#pragma once

#include "Common.h"

class SoundCore
{
	ALCdevice * device;
	ALCcontext * context;
public:
	SoundCore( )
	{
		device = alcOpenDevice( "Generic Hardware" );
		context = alcCreateContext( device, 0 );
		alcMakeContextCurrent( context );
	};

	~SoundCore() 
	{
		alcMakeContextCurrent( 0 );

		alcDestroyContext( context );

		if( device )
			alcCloseDevice( device );
	};
};