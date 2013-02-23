#pragma once

#include "Vertex.h"
#include "Material.h"

class Surface
{
protected:
	vector< Point3 > vertices;
	vector< Point3 > normals;
	vector< Point2 > texCoords;

	vector< uint > indices;

	Material * material;

public:
	Surface();
	~Surface( );
	bool addVertex( const Vertex & v );
	bool addVertex( const Point3 & pos, const Point3 & norm, const Point2 & tc );
	Point3 * getVertex( uint num );
	uint getVertexCount();
	void addIndex( uint index );
	uint getIndex( uint num );
	uint getIndexCount();
	void setMaterial( Material * material );
	Material * getMaterial( )
	{
		return material;
	};
	void render();
};
