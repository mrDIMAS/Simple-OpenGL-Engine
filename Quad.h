#pragma once

#include "Mesh.h"

class Quad : public Mesh
{
public:
	Quad( )
	{
		Surface * surf = new Surface;

		surf->addVertex( Vertex( -1, -1, 0, 0, 0, 0, 0, 0 ));
		surf->addVertex( Vertex(  1, -1, 0, 0, 0, 0, 1, 0 ));
		surf->addVertex( Vertex(  1,  1, 0, 0, 0, 0, 1, 1 ));
		surf->addVertex( Vertex( -1,  1, 0, 0, 0, 0, 0, 1 ));

		//surf->addIndex( 0 ); surf->addIndex( 1 ); surf->addIndex( 2 );
		//surf->addIndex( 0 ); surf->addIndex( 2 ); surf->addIndex( 3 );

		addSurface( surf );
	}
};