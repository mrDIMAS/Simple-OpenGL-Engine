#pragma once

#include "PickObject.h"

class PickAABB : public PickObject
{
	btVector3 mmin, mmax;

public:
	PickAABB( const btVector3 &min,const btVector3 &max )
	{
		mmin = min;
		mmax = max;
	};

	bool intersectByRay( Ray * ray )
	{
		btVector3 halfSize = ( mmax - mmin ) / 2;

		ray->direction.normalize();

		btVector3 P = ray->begin - *pos;

		float l = P.dot( ray->direction );

		btVector3 L = P - ray->direction * l;		

		float k0 = L.dot( L );

		float k1 = ( L.absolute()).dot( halfSize );

		return k0 * k0 * k0 <= k1 * k1;		
	}
};