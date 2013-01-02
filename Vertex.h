#pragma once

class Vertex
{
public:
	float x,y,z;
	float nx,ny,nz;
	float tx,ty;

	Vertex(  );
	Vertex( float x, float y, float z, float nx, float ny, float nz, float tx, float ty );
	bool operator == ( const Vertex & vertex );
};