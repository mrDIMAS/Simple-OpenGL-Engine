#include "Surface.h"

Surface::Surface()
{
	texture = 0;
};

Surface::~Surface( )
{
	for( uint i = 0; i < vertices.size(); i++ )
	{
		delete vertices.at( i );
	};

	if( texture )
		delete texture;
};

bool Surface::addVertex( Vertex * v )
{
	uint vertexIndex  = 0;
	bool foundIdentic = false;

	for( uint i = 0; i < vertices.size(); i++ )
	{
		if( (*vertices.at( i )) == ( *v ) )
		{
			vertexIndex = i ;

			foundIdentic = true;
		}
	};

	if( foundIdentic == false )
	{
		vertexIndex = vertices.size();

		vertices.push_back( v );				
	}

	indices.push_back( vertexIndex );

	return foundIdentic;
};

Vertex * Surface::getVertex( uint num )
{
	return vertices[ num ];
};

uint Surface::getVertexCount()
{
	return vertices.size();
};
	
void Surface::addIndex( uint index )
{
	indices.push_back( index );
};

uint Surface::getIndex( uint num )
{
	return indices[ num ];
};

uint Surface::getIndexCount()
{
	return indices.size();
};

void Surface::setTexture( Texture * texture )
{
	this->texture = texture;
}

void Surface::render()
{
	if( texture )
		texture->bind();

	glBegin( GL_TRIANGLES );

	for( uint i = 0; i < indices.size(); i += 3 )
	{
		for( uint indexNum = 0; indexNum < 3; indexNum++ )
		{
			if( i + indexNum >= indices.size())
				int gfgzdfg= 0;

			Vertex * v = vertices[ indices[ i + indexNum ]];

			glTexCoord3f( v->tx, v->ty, 1 );
			glNormal3f( v->nx, v->ny, v->nz );
			glVertex3f( v->x, v->y, v->z );
		}
	};		

	glEnd();

	glBindTexture( GL_TEXTURE_2D, 0 );
};