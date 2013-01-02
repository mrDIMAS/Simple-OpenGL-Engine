#include "Vertex.h"
#include <math.h>

#define EPSILON 0.00000001

bool equals( float x1, float x2 )
{
	return ( abs( x1 - x2 ) < EPSILON );		
};

Vertex::Vertex(  )
{
	x = y = z = nx = ny = nz = tx = ty = 0;
};

Vertex::Vertex( float x, float y, float z, float nx, float ny, float nz, float tx, float ty )
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
	this->tx = tx;
	this->ty = ty;
};

bool Vertex::operator == ( const Vertex & vertex )
{
	return equals(  x, vertex.x  ) && equals(  y, vertex.y  ) && equals(  z, vertex.z  ) &&
			equals( nx, vertex.nx ) && equals( ny, vertex.ny ) && equals( nz, vertex.nz ) &&
			equals( tx, vertex.tx ) && equals( ty, vertex.ty ) ;
};