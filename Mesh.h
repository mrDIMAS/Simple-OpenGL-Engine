#pragma once

#include "SceneNode.h"
#include "Surface.h"
#include "Light.h"
#include "PickAABB.h"

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

	void parsePropBuffer( Mesh * mesh, string propBuffer )
	{
		Dynamic::Parser::parseValues( propBuffer, mesh->properties );
	};

	void applyProperties( Mesh * mesh )
	{
		for( auto it = mesh->properties.begin(); it != mesh->properties.end(); it++ )
		{
			string pname = it->first;
			Value * val = &it->second;

			if( pname == "name" )
			{
				mesh->setName( val->getStr() );
			};

			if( pname == "visible" )
			{
				if( val->getStr() == "true" || val->getStr() == "1" )
					mesh->show();
				else
					mesh->hide();
			};

			if( pname == "twosided" )
			{
				int parm = val->getNum();

				if( parm )
					mesh->setTwosidedRender();
			};

			if( pname == "pickbody" )
			{
				if( val->getStr() == "sphere" )
				{
					mesh->setPickObject( new PickSphere( mesh->getBoundingSphereRadius() ));
				};

				if( val->getStr() == "aabb" )
				{
					btVector3 min, max;
					mesh->getBoundingAABBMetrics( min, max );
					mesh->setPickObject( new PickAABB( min, max ));
				};
			};

			if( pname == "body" )
			{
				if( val->getStr() == "static" )
				{
					mesh->setRigidBody( new TriMeshBody( mesh->getSurfaces()) );
				};

				if( val->getStr() == "convex" )
				{
					mesh->setRigidBody( new ConvexBody( 1, mesh->getSurfaces()) );
				};
			};

			if( pname == "type" )
			{
				if( val->getStr() == "light" )
				{
					float range = 24;

					PointLight * lit = new PointLight;
					lit->setDiffuseColor( Color( 255, 255, 255, 255 ));
					lit->setRange( range );

					for( auto tit = mesh->properties.begin(); tit != mesh->properties.end(); tit++ )
					{
						string parname = tit->first;
						Value * pval = &tit->second;

						if( parname == "range" )
						{
							lit->setRange( pval->getNum() );
						};
					};

					lit->attachTo( mesh );
				}

				if( val->getStr() == "sound" )
				{
//					SoundChannel * sound = ( new Sound( getProperty( "source" ).getStr()))->play( 
				}
			}
		}

		if( mesh->getRigidBody() )
		{
			for( auto it = mesh->properties.begin(); it != mesh->properties.end(); it++ )
			{
				string pname = it->first;
				Value * val = &it->second;

				if( pname == "mass" )
				{
					mesh->getRigidBody()->setMass( val->getNum() );
				};
			}
		}
	};
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