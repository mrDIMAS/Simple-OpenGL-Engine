#pragma once

#include "SceneNode.h"

class Listener : public SceneNode
{
public:
	Listener() 
	{

	};

	virtual void render()
	{
		btVector3 position	= globalTransform.getOrigin(); float fpos[ 3 ] = { position.x(), position.y(), position.z() };
		btVector3 up		= globalTransform.getBasis().getColumn( 1 ).normalized(); 
		btVector3 look		= globalTransform.getBasis().getColumn( 2 ).normalized(); 

		float forient[ 6 ] = { look.x(), look.y(), look.z(), 0, 1, 0 };

		alListenerfv( AL_POSITION, fpos );
		alListenerfv( AL_ORIENTATION, forient );
	};
};