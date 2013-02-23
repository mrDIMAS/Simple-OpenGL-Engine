#pragma once

#include "PickObject.h"

class PickSphere : public PickObject
{
public:	
	float radius;

	PickSphere( float r  )
	{
		radius = r;
	};

	bool intersectByRay( Ray * ray )
	{
		btVector3 k = ray->begin - *pos;
		float b = k.dot( ray->direction );
		float c = k.dot( k ) - radius * radius;
		float d = b * b - c;

		if(d >=0 )
		{
			float sqrtfd = sqrtf(d);

			float t1 = -b + sqrtfd;
			float t2 = -b - sqrtfd; 

			float min_t = min(t1,t2);
			float max_t = max(t1,t2); 

			float t = (min_t >= 0) ? min_t : max_t;

			return (t > 0);
		}

		return 0;
	};
};