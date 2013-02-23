#include "Surface.h"

Surface::Surface()
{
	material = 0;
};

Surface::~Surface( )
{
	vertices.clear();

	if( material )
		delete material;
};

bool Surface::addVertex( const Vertex & v )
{
	uint vertexIndex  = 0;
	bool foundIdentic = false;

	for( uint i = 0; i < vertices.size(); i++ )
	{
		if( ( vertices.at( i )  == Point3( v.x, v.y, v.z ) )    &&
			( normals.at( i )   == Point3( v.nx, v.ny, v.nz ) ) && 
			( texCoords.at( i ) == Point2( v.tx, v.ty ) ) )
		{
			vertexIndex = i ;

			foundIdentic = true;
		}
	};

	if( foundIdentic == false )
	{
		vertexIndex = vertices.size();

		vertices.push_back( Point3( v.x, v.y, v.z ) );		
		normals.push_back( Point3( v.nx, v.ny, v.nz ) );
		texCoords.push_back( Point2( v.tx, v.ty ));
	}

	indices.push_back( vertexIndex );
	
	return foundIdentic;
};

bool Surface::addVertex( const Point3 & pos, const Point3 & norm, const Point2 & tc )
{
	uint vertexIndex  = 0;
	bool foundIdentic = false;

	for( uint i = 0; i < vertices.size(); i++ )
	{
		if( ( vertices.at( i )  == pos ) &&
			( normals.at( i )   == norm) && 
			( texCoords.at( i ) == tc ) )
		{
			vertexIndex = i ;

			foundIdentic = true;
		}
	};

	if( foundIdentic == false )
	{
		vertexIndex = vertices.size();

		vertices.push_back( pos );		
		normals.push_back( norm );
		texCoords.push_back( tc );
	}

	indices.push_back( vertexIndex );
	
	return foundIdentic;
};

Point3 * Surface::getVertex( uint num )
{
	return &vertices[ num ];
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

void Surface::setMaterial( Material * material )
{
	this->material = material;
}

void Surface::render()
{
	if( material )
		material->bind();

	if( vertices.size() && indices.size() )
	{
		glEnableClientState( GL_VERTEX_ARRAY );
		glEnableClientState( GL_NORMAL_ARRAY );
		glEnableClientState( GL_TEXTURE_COORD_ARRAY );

		glVertexPointer  ( 3, GL_FLOAT, 0, &vertices [ 0 ] );
		glNormalPointer  (    GL_FLOAT, 0, &normals  [ 0 ] );
		glTexCoordPointer( 2, GL_FLOAT, 0, &texCoords[ 0 ] );
		
		glDrawElements( GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)&indices[ 0 ] );		
	};

	glBindTexture( GL_TEXTURE_2D, 0 );
};