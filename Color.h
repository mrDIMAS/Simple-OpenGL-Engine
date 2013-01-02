#pragma once

class Color
{
private:
	union
	{
		struct
		{
			float r, g, b, a;
		};

		float c[ 4 ];
	};
public:
	Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
	{
		r = red / 255.0f;
		g = green / 255.0f;
		b = blue / 255.0f;
		a = alpha / 255.0f;
	};
	
	operator float * ()
	{
		return c;
	};

	float getRed()
	{
		return r;
	};

	float getGreen()
	{
		return g;
	};

	float getBlue()
	{
		return b;
	};
};