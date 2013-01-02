#pragma once

#include "Vertex.h"
#include "Texture.h"

class Surface
{
protected:
	vector< Vertex* > vertices;
	vector< uint > indices;
	Texture * texture;

public:
	Surface();
	~Surface( );
	bool addVertex( Vertex * v );
	Vertex * getVertex( uint num );
	uint getVertexCount();
	void addIndex( uint index );
	uint getIndex( uint num );
	uint getIndexCount();
	void setTexture( Texture * texture );
	void render();
};
