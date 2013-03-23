#include "Light.h"

Light::Light() 
{
	id = GL_LIGHT0 + count;

	count++;

	enable();
	setDiffuseColor( Color( 255, 255, 255, 255 ));
	setSpecularColor( Color( 255, 255, 255, 255 ));
	setAmbientColor( Color( 80, 80, 80, 80 ));
	setLinearAttenuation( 0 );
	setConstantAttenuation( 1 );
	setQuadraticAttenuation( 0 );
	setDirection( Vector4( 0, 0, 0, 0 ));
};

void Light::enable( )
{
	glEnable( id );
};

void Light::disabled( )
{
	glDisable( id );
};

void Light::setJitter( float j )
{
	jitterFactor = j;
};

void Light::setDiffuseColor( Color & color )
{
	glLightfv( id, GL_DIFFUSE,  color );
};

void Light::setDirection( Vector4 dir  )
{
	glLightfv( id, GL_SPOT_DIRECTION, dir );
};

void Light::setSpecularColor( Color & color )
{
	glLightfv( id, GL_SPECULAR,  color );
};

void Light::setAmbientColor( Color & color )
{
	glLightfv( id, GL_AMBIENT,  color );
};

void Light::setLinearAttenuation( float att )
{
	glLightf( id, GL_LINEAR_ATTENUATION, att );
};

void Light::setConstantAttenuation( float att )
{
	glLightf( id, GL_CONSTANT_ATTENUATION, att );
};

void Light::setQuadraticAttenuation( float att )
{
	glLightf( id, GL_QUADRATIC_ATTENUATION, att );
};

void Light::setRange( float r )
{
	setConstantAttenuation( 0 );
	setLinearAttenuation( 1.0f / r );
	setQuadraticAttenuation( 5.0f / ( r * r ) );
};

unsigned char Light::count = 0;

void DirectionalLight::render()
{
	float vpos[ 4 ] = { 0, 0, 1, 0 };

	applyTransform();

	glLightfv( id, GL_POSITION, vpos );		
};

void PointLight::render()
{
	float vpos[ 4 ] = { 0, 0, 0, 1 };
	
	applyTransform();

	glLightfv( id, GL_POSITION, vpos );		
};

void SpotLight::render()
{
	float vpos[ 4 ] = { 0, 0, 0, 1 };
	float vdir[ 3 ] = { 0, 0, 0 };

	applyTransform();

	glLightfv( id, GL_POSITION, vpos );	
	glLightfv( id, GL_SPOT_DIRECTION, vdir );		
};

void SpotLight::setCutoffAngle( float a )
{
	glLightf( id, GL_SPOT_CUTOFF, a );
};

void SpotLight::setExp( float e )
{
	glLightf( id, GL_SPOT_EXPONENT, e ); 
};
