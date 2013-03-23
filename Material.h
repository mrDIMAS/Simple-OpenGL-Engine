#pragma once

#include "Texture.h"

class Material
{
protected:
	Texture * texture;
	Color ambient, diffuse, specular, emission;
	float shininess;
public:
	Material() : ambient( 22, 22, 22, 255 ), diffuse( 255, 255, 255, 255 ), specular( 0, 0, 0, 255 ), emission( 0, 0, 0, 255 )
	{
		texture   = 0;
		shininess = 0;
	};

	void setAmbientColor( const Color & color )
	{
		ambient = color;		
	};

	void setEmissionColor( const Color & color )
	{
		emission = color;		
	};

	void setDiffuseColor( const Color & color )
	{
		diffuse = color;		
	};

	void setSpecularColor( const Color & color )
	{
		specular = color;		
	};

	void setShininess( float shininess )
	{	
		this->shininess = shininess;
	};

	void setTexture( Texture * texture )
	{
		this->texture = texture;
	};

	void bind()
	{
		if( texture )
			texture->bind();

		glMaterialfv( GL_FRONT, GL_AMBIENT,  ambient.getElements() );
		glMaterialfv( GL_FRONT, GL_DIFFUSE,  diffuse.getElements() );
		glMaterialfv( GL_FRONT, GL_SPECULAR, specular.getElements() );
		glMaterialfv( GL_FRONT, GL_EMISSION, emission.getElements() );
		glMaterialf(  GL_FRONT, GL_SHININESS, shininess );
	};
};