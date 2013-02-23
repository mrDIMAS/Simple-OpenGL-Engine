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


class Point3
{
public:
	union
	{
		struct
		{
			float x, y, z;
		};

		float components[ 3 ];
	};

	Point3( float x, float y, float z )
	{
		this->x = x;
		this->y = y;
		this->z = z;
	};

	operator float * ()
	{
		return components;
	};

	bool operator == ( const Point3 & point )
	{
		return ( abs( x - point.x ) < 0.0001 ) && ( abs( y - point.y ) < 0.0001 ) && ( abs( z - point.z ) < 0.0001 );
	};
};

class Point2
{
public:
	union
	{
		struct
		{
			float x, y;
		};

		float components[ 2 ];
	};

	Point2( float x, float y )
	{
		this->x = x;
		this->y = y;
	};

	operator float * ()
	{
		return components;
	};

	bool operator == ( const Point2 & point )
	{
		return ( abs( x - point.x ) < 0.0001 ) && ( abs( y - point.y ) < 0.0001 );
	};
};