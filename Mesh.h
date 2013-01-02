#pragma once

#include "SceneNode.h"
#include "Surface.h"

class Mesh : public SceneNode
{
protected:
	vector< Surface* > surfaces;

	string name;

	virtual void render( )
	{
		applyTransform();

		for( uint i = 0; i < surfaces.size(); i++ )
		{
			surfaces[ i ]->render();
		};			
	};

public:
	Mesh()
	{
		
	};

	~Mesh( )
	{
		for( uint i = 0; i < surfaces.size(); i++ )
		{
			delete surfaces.at( i );
		}
	};

	void addSurface( Surface * surface )
	{
		surfaces.push_back( surface );
	};
	
	string & getName( )
	{
		return name;
	};

	void setTexture( Texture * texture )
	{
		for( uint i = 0; i < surfaces.size(); i++ )
		{
			surfaces[ i ]->setTexture( texture );
		};
	};

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
	void loadEO( const char * filename )
	{
		uint time = SDL_GetTicks();

		FILE * file = fopen( filename, "rb" );

		ulong numObjects;
		fread( &numObjects, sizeof( numObjects ), 1, file );

		for( ulong objNum = 0; objNum < numObjects; objNum++ )
		{
			// common data
			vector< uint >  faces;
			vector< uint >  texturedFaces;
			vector< Vector4 > textureCoords;
			vector< Vector4 > vertexCoords;
			vector< Vector4 > vertexNormals;
			vector< int > faceMatId;

			Mesh * mesh = this; // first object in object became root

			if( objNum > 0 ) // otherwise next object became as a child of root
				mesh = new Mesh;			


			ulong vertexCount;			fread( &vertexCount, sizeof( vertexCount ), 1, file );
			ulong textureCoordCount;	fread( &textureCoordCount, sizeof( textureCoordCount ), 1, file );
			ulong faceCount;			fread( &faceCount, sizeof( faceCount ), 1, file );							

			float px, py, pz, rx, ry, rz;

			// read position
			fread( &px, sizeof( px ), 1, file );
			fread( &py, sizeof( py ), 1, file );
			fread( &pz, sizeof( pz ), 1, file );

			// read euler angles
			fread( &rx, sizeof( rx ), 1, file );
			fread( &ry, sizeof( ry ), 1, file );
			fread( &rz, sizeof( rz ), 1, file );

			// read name
			char symbol; fread( &symbol, sizeof( symbol ), 1, file );		
		
			while( symbol )
			{
				mesh->getName().push_back( symbol );

				fread( &symbol, sizeof( symbol ), 1, file );
			};

			ulong numMaterials; fread( &numMaterials, sizeof( numMaterials ), 1, file );

			// read diffuse texture name

			vector< string > textures;//string difTexture = "";
					
			for( ulong i = 0; i < numMaterials; i++ )
			{
				string difTexture;

				fread( &symbol, sizeof( symbol ), 1, file );	

				while( symbol )
				{
					difTexture.push_back( symbol );

					fread( &symbol, sizeof( symbol ), 1, file );
				};

				textures.push_back( difTexture );
			}
								
			for( ulong i = 0; i < vertexCount; i++ )
			{
				// read vertex coords	
				float x; fread( &x, 1, sizeof( x ), file );
				float y; fread( &y, 1, sizeof( y ), file );
				float z; fread( &z, 1, sizeof( z ), file );

				vertexCoords.push_back( Vector4( x, y, z, 0 ) );

				// read vertex normals
				fread( &x, 1, sizeof( x ), file );
				fread( &y, 1, sizeof( y ), file );
				fread( &z, 1, sizeof( z ), file );

				vertexNormals.push_back( Vector4( x, y, z, 0 ) );
			};
			
			// read texture coords
			for( ulong i = 0; i < textureCoordCount; i++ )
			{
				float x; fread( &x, 1, sizeof( x ), file );
				float y; fread( &y, 1, sizeof( y ), file );

				textureCoords.push_back( Vector4( x, y, 0, 0 ) );
			};
						
			for( ulong i = 0; i < faceCount; i++ )
			{
				// read indices	
				for( uint j = 0; j < 3; j++ )
				{
					ulong index; fread( &index, sizeof( index ), 1, file );	
					faces.push_back( index );
				}

				// read texture indices	
				for( uint j = 0; j < 3; j++ )
				{
					ulong textureIndex; fread( &textureIndex, sizeof( textureIndex ), 1, file );	
					texturedFaces.push_back( textureIndex );
				}

				ulong faceMat; fread( &faceMat, sizeof( faceMat ), 1, file );	

				faceMatId.push_back( faceMat );
			}

			if( numMaterials == 0 ) // no texture
			{
				Surface * surface = new Surface;

				// construct vertices
				for( uint i = 0; i < faces.size(); i++ )
				{
					Vector4 pos  = vertexCoords.at( faces[ i ] );				
					Vector4 norm = vertexNormals.at( faces[ i ] );
					Vector4 tc   = textureCoords.at( texturedFaces[ i ] );

					Vertex * vertex = new Vertex( pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tc.x, tc.y );

					bool foundIdentic = surface->addVertex( vertex );

					if( foundIdentic )
					{
						delete vertex;
					};		
				}

				mesh->addSurface( surface );
			}
			else // sort surfaces by textures
			{
				for( uint matID = 0; matID < numMaterials; matID++ )
				{
					Surface * surface = new Surface;

					uint vn = 0;
					uint fn = 0;

					// construct vertices
					for( uint i = 0; i < faces.size(); i++ )
					{
						if( numMaterials == 1 )
						{
							Vector4 pos  = vertexCoords.at( faces[ i ] );				
							Vector4 norm = vertexNormals.at( faces[ i ] );
							Vector4 tc   = textureCoords.at( texturedFaces[ i ] );

							Vertex * vertex = new Vertex( pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tc.x, tc.y );

							bool foundIdentic = surface->addVertex( vertex );

							if( foundIdentic )
							{
								delete vertex;
							};	
						}
						else
						{
							if( faceMatId.at( fn ) == matID )
							{
								Vector4 pos  = vertexCoords.at( faces[ i ] );				
								Vector4 norm = vertexNormals.at( faces[ i ] );
								Vector4 tc   = textureCoords.at( texturedFaces[ i ] );

								Vertex * vertex = new Vertex( pos.x, pos.y, pos.z, norm.x, norm.y, norm.z, tc.x, tc.y );

								bool foundIdentic = surface->addVertex( vertex );

								if( foundIdentic )
								{
									delete vertex;
								};	
							}

							vn++;

							if( vn == 3 )
							{
								vn = 0;

								fn++;
							}
						}
					}
					
					if( textures.size() )
					{
						if( textures.at( matID ).size() )
						{
							Texture * texture =	new Texture();

							texture->loadTGA( textures.at( matID ).c_str() );

							surface->setTexture( texture );
						}
					}

					mesh->addSurface( surface );
				}
			}

			mesh->setPosition( btVector3( px, py, pz ) );
			mesh->setRotation( btVector3( rx, ry, rz ) );

			if( objNum > 0 )
				mesh->attachTo( this );
		}

		fclose( file );

		cout << SDL_GetTicks( ) - time << endl;
	};

	vector< Surface* > * getSurfaces( )
	{
		return &surfaces;
	};

	float getBoundingSphereRadius( )
	{
		btVector3 min = btVector3( FLT_MAX, FLT_MAX, FLT_MAX ), max = btVector3( -FLT_MAX, -FLT_MAX, -FLT_MAX );

		for( int childNum = -1; childNum < (int)childs.size(); childNum++ )
		{
			Mesh * mesh = 0;

			if( childNum == -1 )
			{
				mesh = this;
			}
			else
			{
				mesh = ( Mesh * )childs.at( childNum );

				if( !mesh )
					continue;
			};

			for( uint i = 0; i < mesh->surfaces.size(); i++ )
			{
				Surface * surface = mesh->surfaces.at( i );		

				for( uint j = 0; j < surface->getVertexCount(); j++ )
				{
					Vertex * v = surface->getVertex( j );

					// max
					if( v->x > max.x() )
						max.setX( v->x );
					if( v->y > max.y() )
						max.setY( v->y );
					if( v->z > max.z() )
						max.setZ( v->z );
					// min
					if( v->x < min.x() )
						min.setX( v->x );
					if( v->y < min.y() )
						min.setY( v->y );
					if( v->z < min.z() )
						min.setZ( v->z );
				};
			};
		};

		return ( max - min ).length( ) / 2;
	};
};