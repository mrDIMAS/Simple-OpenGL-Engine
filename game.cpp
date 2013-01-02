/*

	OGLShooter

	by mr.dimas 2013

*/

#include "Engine.h"

int main(int argc, char *argv[])
{	
	Renderer * renderer = new Renderer( 1 );

	Mesh * scene = new Mesh;
	scene->loadEO( "level.eo" );
	scene->setRigidBody( new TriMeshBody( scene->getSurfaces()));

	for( uint i = 0; i < scene->getChildCount(); i++ )
	{
		scene->getChild( i )->setRigidBody( new ConvexBody( 1, ((Mesh*)scene->getChild( i ))->getSurfaces()));
	};

	Pivot * pivot = new Pivot;
	pivot->attachTo( scene );
	pivot->setPosition( btVector3( 0, 5, 0 ));
	pivot->setRigidBody( new SphereBody( 1, 2.5 ));

	pivot->getRigidBody()->getPhysBody()->setAngularFactor( btVector3( 0, 0, 0 ));
	pivot->getRigidBody()->getPhysBody()->setAnisotropicFriction( btVector3( 1, 0, 1 ));

	Camera * camera = new Camera;
	camera->setClearColor( Color( 255, 255, 255, 255 ));
	camera->attachTo( pivot ); 
	camera->setPosition( btVector3( 0, 2.5, 0 ));

	PointLight * pointLight = new PointLight;
	pointLight->setDiffuseColor( Color( 255, 255, 255, 255 ));
	pointLight->setLinearAttenuation( 0.05 );
	pointLight->setQuadraticAttenuation( 0.005 );
	pointLight->attachTo( camera );

	const float sp = 10;

	while( true )
	{
		renderer->update();

		renderer->beginRender();
			
		scene->renderNodeAndChilds();

		drawRightHandedGizmoXYZWhereZLookToward();
		
		float speed = 0;
		float strafe = 0;

		if( keyboard.keyPressed( SDLK_w ) )
			speed = sp;
		if( keyboard.keyPressed( SDLK_s ) )
			speed = -sp;
		if( keyboard.keyPressed( SDLK_a ) )
			strafe = -sp;
		if( keyboard.keyPressed( SDLK_d ) )
			strafe =  sp;

		if( keyboard.keyPressed( SDLK_SPACE ))
			pivot->getRigidBody( )->addForce( btVector3( 0, 200, 0 ));

		if( keyboard.keyPressed( SDLK_ESCAPE ))
			break;

		camera->turn( btVector3( -mouse.getYSpeed() * 0.2, 0, 0 ));		
		pivot->turn( btVector3( 0, mouse.getXSpeed() * 0.2, 0 ));
		pivot->move( btVector3( strafe, 0, speed ));

		mouse.moveTo( 200, 200 );
		
		mouse.update();

		renderer->endRender( true, false );
	};

	delete scene;

	delete renderer;
	
	return 0;
}