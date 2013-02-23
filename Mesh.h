#pragma once

#include "SceneNode.h"
#include "Surface.h"

/*
EO file format specification v0.1

file begin

ulong numObjects

for i = 0 to numObjects
{
	ulong vertexCount
	ulong textureCoordCount
	ulong faceCount

	float px
	float py
	float pz

	float rx
	float ry
	float rz

	string name ends 0-byte

	ulong numMaterials

	for j = 0 to numMaterials
	{
		string textureName ends 0-byte
	}

	for vc = 0 to vertexCount
	{
		float x
		float y
		float z

		float nx
		float ny
		float nz
	}

	for tc = 0 to textureCoordCount
	{
		float tx
		float ty
	}

	for fc = 0 to faceCount
	{
		uint face_vertex_index1
		uint face_vertex_index2
		uint face_vertex_index3

		uint face_texture_index1
		uint face_texture_index2
		uint face_texture_index3

		uint faceMaterialId
	}
}

file end

surface assembly

for i = 0 to 

*/
enum
{
	RF_TWOSIDED = 1,
};

static string getPathFromFromFileName( string fname )
{
	string result = fname.substr( 0, fname.find_last_of( '\\' ) + 1 );
	return result;
};

class Mesh : public SceneNode
{
protected:
	vector< Surface* > surfaces;

	

	virtual void render( );
	int flags;

public:
	Mesh();
	~Mesh( );
	void addSurface( Surface * surface );
	
	void setTexture( Texture * texture );

	void loadEO( const char * filename );
	vector< Surface* > * getSurfaces( );
	float getBoundingSphereRadius( );
	void getBoundingAABBMetrics( btVector3 &min, btVector3 &max );
	void setTwosidedRender()
	{
		flags &= RF_TWOSIDED;
	};
};