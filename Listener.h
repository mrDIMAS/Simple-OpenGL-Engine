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
		btVector3 up		= globalTransform.getBasis().getColumn( 1 ).normalize(); 
		btVector3 look		= globalTransform.getBasis().getColumn( 0 ).normalize(); 

		float forient[ 6 ] = { look.x(), look.y(), look.z(), up.x(), up.y(), up.z() };

		alListenerfv( AL_POSITION, fpos );
		alListenerfv( AL_ORIENTATION, forient );
	};
};