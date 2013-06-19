#include "Mesh.h"

void Mesh::render( )
{
	if( globalRenderFlags & GRF_USELIGHT )
	{
		glEnable( GL_LIGHTING );
	}

	if( shader )
	{
		shader->bind();

		glActiveTexture( GL_TEXTURE0 );		
		int texLoc = glGetUniformLocation( shader->program_id, "diffTex" );
		glUniform1i( texLoc, 0);
	};

	applyTransform();
	
	if( flags & RF_TWOSIDED )
	{
		glDisable( GL_CULL_FACE );
		glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );
	};

	if( flags & RF_DONTUSELIGHT )
	{
		glDisable( GL_LIGHTING );
	}

	for( uint i = 0; i < surfaces.size(); i++ )
	{
		surfaces[ i ]->render();
	};		

	if( globalRenderFlags & GRF_USELIGHT )
	{
		glEnable( GL_LIGHTING );
	}
	
	if( shader )
		Shader::unbind();
};

Mesh::Mesh()
{
	flags = 0;
};

Mesh::~Mesh( )
{
	for( uint i = 0; i < surfaces.size(); i++ )
	{
		delete surfaces.at( i );
	}
};

void Mesh::addSurface( Surface * surface )
{
	surfaces.push_back( surface );
};

void Mesh::setTexture( Texture * texture )
{
	for( uint i = 0; i < surfaces.size(); i++ )
	{
		surfaces[ i ]->getMaterial( )->setTexture( texture );
	};
};
	
void Mesh::loadEO( const char * filename ) // can be very slow in debug-mode! be careful
{
	string pathToRes = getPathFromFromFileName( filename );

	uint time = SDL_GetTicks();

	FILE * file = fopen( filename, "rb" );

	if( !file )
		return;

	ulong numObjects;
	fread( &numObjects, sizeof( numObjects ), 1, file );

	for( ulong objNum = 0; objNum < numObjects; objNum++ )
	{
		// common data
		vector< uint >  faces;
		vector< uint >  texturedFaces;
		vector< Point2 > textureCoords;
		vector< Point3 > vertexCoords;
		vector< Point3 > vertexNormals;
		vector< int > faceMatId;

		Mesh * mesh = this; // first object in object became root

		if( objNum > 0 ) // otherwise next object became as a child of root
			mesh = new Mesh;

		ulong upCount;			fread( &upCount, sizeof( upCount ), 1, file );

		char symbol; fread( &symbol, sizeof( symbol ), 1, file );				
		string propBuffer;
		while( symbol )
		{
			propBuffer.push_back( symbol );

			fread( &symbol, sizeof( symbol ), 1, file );
		};

		parsePropBuffer( mesh, propBuffer );

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
		fread( &symbol, sizeof( symbol ), 1, file );		
		
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

		float temp[ 6 ] = { 0 };
								
		for( ulong i = 0; i < vertexCount; i++ )
		{
			fread( temp, 1, sizeof( temp ), file );
			vertexCoords.push_back( Point3( temp[ 0 ], temp[ 1 ], temp[ 2 ] ) );
			vertexNormals.push_back( Point3( temp[ 3 ], temp[ 4 ], temp[ 5 ] ) );
		};
			
		// read texture coords
		for( ulong i = 0; i < textureCoordCount; i++ )
		{
			fread( temp, 1, 2 * sizeof( float ), file );

			textureCoords.push_back( Point2( temp [ 0 ], temp[ 1 ] ) );
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
				surface->addVertex( vertexCoords.at( faces[ i ] ),  vertexNormals.at( faces[ i ] ), textureCoords.at( texturedFaces[ i ] ) );
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
						surface->addVertex( vertexCoords.at( faces[ i ] ),  vertexNormals.at( faces[ i ] ), textureCoords.at( texturedFaces[ i ] ) );
					}
					else
					{
						if( faceMatId.at( fn ) == matID )
						{
							surface->addVertex( vertexCoords.at( faces[ i ] ),  vertexNormals.at( faces[ i ] ), textureCoords.at( texturedFaces[ i ] ) );
						}

						vn++;

						if( vn == 3 )
						{
							vn = 0;

							fn++;
						}
					}
				}

				Material * material = new Material;

				if( textures.size() )
				{
					if( textures.at( matID ).size() )
					{
						material->setTexture( Texture::loadTGA( ( pathToRes + textures.at( matID ) ).c_str(), 1 ) );
					}
				}

				surface->setMaterial( material );

				mesh->addSurface( surface );
			}
		}

		if( objNum > 0 )
			mesh->attachTo( this, 0 );

		mesh->setGlobalPosition( btVector3( px, py, pz ) );
		mesh->setRotation( btVector3( rx, ry, rz ) );

		applyProperties( mesh );
	}

	fclose( file );

	cout << SDL_GetTicks( ) - time << endl;
};

vector< Surface* > * Mesh::getSurfaces( )
{
	return &surfaces;
};

void Mesh::getBoundingAABBMetrics( btVector3 &min, btVector3 &max )
{
	min = btVector3( FLT_MAX, FLT_MAX, FLT_MAX );
	max = btVector3( -FLT_MAX, -FLT_MAX, -FLT_MAX );

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
				Point3 * v = surface->getVertex( j );

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
}

float Mesh::getBoundingSphereRadius( )
{
	btVector3 min, max;

	getBoundingAABBMetrics( min, max );

	return ( max - min ).length( ) / 2;
};