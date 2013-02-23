#pragma once

#include "Common.h"

class Ray
{
public:
	btVector3 direction;
	btVector3 begin;

	Ray( const btVector3 & begin, const btVector3 & end )
	{
		setup( begin, end );
	};
	
	Ray( )
	{
		begin	  = btVector3( 0, 0, 0 );
		direction = btVector3( 0, 0, 1 );
	};

	void setup( const btVector3 & begin, const btVector3 & end )
	{
		this->begin = begin;

		direction = ( end - begin ).normalize();
	};
};

class PickObject
{
public:
	btVector3 * pos;

	PickObject ()
	{
	};

	virtual bool intersectByRay( Ray * ray ) = 0;
};