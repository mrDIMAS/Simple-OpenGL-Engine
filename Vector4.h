#pragma once

class Vector4
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float components[ 4 ];
	};

	Vector4( float x, float y, float z, float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};

	operator float * ()
	{
		return components;
	};
};