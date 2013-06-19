#pragma once

#include "SceneNode.h"

class Light : public SceneNode
{
protected:
	GLenum id;
	static unsigned char count;	
	float jitterFactor;
	float range;
public:
	Light();
	void enable( );
	void disabled( );
	void setDiffuseColor( Color & color );
	void setDirection( Vector4 dir  );
	void setSpecularColor( Color & color );
	void setAmbientColor( Color & color );
	void setLinearAttenuation( float att );
	void setConstantAttenuation( float att );
	void setQuadraticAttenuation( float att );
	void setRange( float r );
	void setJitter( float j );
	void setRangeMult( float rm )
	{
		setConstantAttenuation( 0 );
		setLinearAttenuation( 1.0f / ( range * rm ) );
		setQuadraticAttenuation( 5.0f / ( range * range * rm ) );
	};
};

class DirectionalLight : public Light
{
public:
	virtual void render();
};

class PointLight : public Light
{
public:
	virtual void render();
};

class SpotLight : public Light
{
public:
	virtual void render();
	void setCutoffAngle( float a );
	void setExp( float e );
};