#pragma once

#include "SceneNode.h"

class Light : public SceneNode
{
protected:
	GLenum id;

	static unsigned char count;
	
public:
	Light() 
	{
		id = GL_LIGHT0 + count;

		count++;

		enable();
		setDiffuseColor( Color( 255, 255, 255, 255 ));
		setSpecularColor( Color( 255, 255, 255, 255 ));
		setAmbientColor( Color( 0, 0, 0, 0 ));
		setLinearAttenuation( 0 );
		setConstantAttenuation( 1 );
		setQuadraticAttenuation( 0 );
		setDirection( Vector4( 0, 0, 0, 0 ));
	};

	void enable( )
	{
		glEnable( id );
	};

	void disabled( )
	{
		glDisable( id );
	};

	void setDiffuseColor( Color & color )
	{
		glLightfv( id, GL_DIFFUSE,  color );
	};

	void setDirection( Vector4 dir  )
	{
		glLightfv( id, GL_SPOT_DIRECTION, dir );
	};

	void setSpecularColor( Color & color )
	{
		glLightfv( id, GL_SPECULAR,  color );
	};

	void setAmbientColor( Color & color )
	{
		glLightfv( id, GL_AMBIENT,  color );
	};

	void setLinearAttenuation( float att )
	{
		glLightf( id, GL_LINEAR_ATTENUATION, att );
	};

	void setConstantAttenuation( float att )
	{
		glLightf( id, GL_CONSTANT_ATTENUATION, att );
	};

	void setQuadraticAttenuation( float att )
	{
		glLightf( id, GL_QUADRATIC_ATTENUATION, att );
	};
};


unsigned char Light::count = 0;

class DirectionalLight : public Light
{
public:
	virtual void render()
	{
		float vpos[ 4 ] = { 0, 0, 1, 0 };

		applyTransform();

		glLightfv( id, GL_POSITION, vpos );		
	};
};

class PointLight : public Light
{
public:
	virtual void render()
	{
		float vpos[ 4 ] = { 0, 0, 0, 1 };
	
		applyTransform();

		glLightfv( id, GL_POSITION, vpos );		
	};
};
