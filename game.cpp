/*

	OGLShooter

	by mr.dimas 2013

*/

#include "Engine.h"

// map parser 
class ProLevel
{	
public:
	Mesh * root;

	ProLevel( const char * file )
	{
		root = new Mesh;

		root->loadEO( file );		

		root->setRigidBody( new TriMeshBody( root->getSurfaces()));

		for( uint i = 0; i < root->getChildCount(); i++ )
		{
			Mesh * mesh = (Mesh*)root->getChild( i );

			if( mesh )
			{
				map<string,string> parameters;

				string name = mesh->getName();

				char * delim = "= ;";
				char * pname = 0;
				char * pvar  = 0;
				bool   begin = false;				

				while( true )
				{
					if( !begin )
					{
						pname = strtok( const_cast<char*>( name.c_str() ), delim );
						pvar  = strtok( 0, delim );

						if( pname && pvar )
							parameters[ pname ] = pvar;
						else
							break;

						begin = true;
					}
					else
					{
						pname = strtok( 0, delim );
						pvar  = strtok( 0, delim );

						if( pname && pvar )
							parameters[ pname ] = pvar;
						else
							break;
					};
				};

				for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
				{
					string pname = parm->first;
					string pvar  = parm->second;

					if( pname == "type" )
					{
						if( pvar == "light" )
						{
							createLight( parameters )->attachTo( mesh );
						};
						if( pvar == "mesh" )
						{
							configureMesh( mesh, parameters );
						};
					};
				};
				//root->getChild( i )->setRigidBody( new ConvexBody( 1, ((Mesh*)root->getChild( i ))->getSurfaces()));
			}
		};
	}

	Light * createLight( map<string,string> & parameters )
	{
		float range = 24;

		PointLight * lit = new PointLight;
		lit->setDiffuseColor( Color( 255, 255, 255, 255 ));
		lit->setAmbientColor( Color( 0, 0, 0, 255 ));
		lit->setRange( range );

		return lit;
	};

	void configureMesh( Mesh * mesh, map<string,string> & parameters )
	{
		for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
		{
			string pname = parm->first;
			string pvar  = parm->second;

			if( pname == "body" )
			{
				if( pvar == "convex" )
				{
					mesh->setRigidBody( new ConvexBody( 1, mesh->getSurfaces()) );
				};
				if( pvar == "static" )
				{
					mesh->setRigidBody( new TriMeshBody( mesh->getSurfaces()) );
				};
			};
		};

		if( mesh->getRigidBody() )
		{
			for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
			{
				string pname = parm->first;
				string pvar  = parm->second;

				if( pname == "mass" )
				{
					mesh->getRigidBody()->setMass( atof( pvar.c_str()));
				};
			};
		};
	};

	~ProLevel( )
	{
		delete root;
	};
};

class Player
{
private:
	Pivot  * pivot;
	Camera * camera;
	vector< Sound * > stepSounds;
	float pathLen;

	void playStepSound()
	{
		if( pathLen > 200 )
		{
			stepSounds.at( rand() % stepSounds.size() )->play( camera );

			pathLen = 0;
		};
	};
	void update( )
	{
		const float sp = 10;
		static float angle = 0;
		static float headOffset = 0;
		static bool jumped = 0;

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

		if( ( abs( speed ) > 0 ) && ( abs( strafe ) > 0 ) )
		{
			strafe *= 0.5;
			speed  *= 0.5;		
		}

		if( keyboard.isLShiftPressed() )
		{
			angle += abs( speed ) + abs( strafe );

			strafe *= 2;
			speed  *= 2;
		}
		else
			angle += abs( speed ) + abs( strafe );

		pathLen += abs( strafe ) + abs( speed );

		playStepSound();
		
		// landing
		if( pivot->getRigidBody()->onGround() && jumped  )
		{
			headOffset -= 0.1;

			if( headOffset < -0.8 )
				jumped = false;
		};

		if( !jumped )
		{
			headOffset += -headOffset * 0.05;
		};

		if( keyboard.keyPressed( SDLK_SPACE ) && pivot->getRigidBody()->onGround() )
		{
			pivot->getRigidBody()->setLinearVelocity( btVector3( 0, 10, 0 ));

			jumped = true;
		};
		
		camera->setPosition( btVector3( 0, headOffset + 2.5 + sinf( angle * 0.02 ) * 0.125, 0 ));

		float mys = -mouse.getYSpeed() * 0.2;

		camera->turn( btVector3( mys, 0, 0 ));		
		pivot->turn( btVector3( 0, mouse.getXSpeed() * 0.2, 0 ));
		pivot->move( btVector3( strafe, 0, speed ));

	};
public:
	friend class World;

	Player( const btVector3 & pos )
	{
		pivot = new Pivot();
		
		pivot->setRigidBody( new SphereBody( 1, 3 ));		
		pivot->getRigidBody()->getPhysBody()->setAngularFactor( btVector3( 0, 0, 0 ));
		pivot->getRigidBody()->getPhysBody()->setAnisotropicFriction( btVector3( 1, 0, 1 ));
		pivot->setPosition( pos );

		camera = new Camera();
		camera->setClearColor( Color( 0, 0, 0, 255 ));
		camera->attachTo( pivot ); 
		camera->setPosition( btVector3( 0, 2.5, 0 ));

		pathLen = 0;

		stepSounds.push_back( new Sound( "step1.ogg" ));
		stepSounds.push_back( new Sound( "step2.ogg" ));
		stepSounds.push_back( new Sound( "step3.ogg" ));
		stepSounds.push_back( new Sound( "step4.ogg" ));
	};

	Camera * getCamera()
	{
		return camera;
	};

	Pivot * getPivot()
	{
		return pivot;
	};
};

class World
{
public:
	ProLevel * curLevel;
	Player   * player;
	SoundCore * sndcore;

	World( )
	{
		curLevel = 0;
		player = 0;
		sndcore = new SoundCore;
	};

	~World( )
	{
		delete curLevel;
		delete player;
		delete sndcore;
	};

	void setLevel( ProLevel * lev )
	{
		curLevel = lev;
	};

	void setPlayer( Player * p )
	{
		player = p;

		player->getPivot()->attachTo( curLevel->root );

		Listener * listener = new Listener;
		listener->attachTo( player->getCamera() );
	};

	void update( )
	{
		curLevel->root->renderNodeAndChilds();

		player->update();

		mouse.moveTo( 200, 200 );
		
		mouse.update();
	};
};

class Game
{
public:

};

int main(int argc, char *argv[])
{	
	Renderer * renderer = new Renderer( 1 );

	World * world = new World;

	Player * player = new Player( btVector3( 0, 5, 0 ));
	world->setLevel( new ProLevel( "level3.eo" ));
	world->setPlayer( player  );
	
	PointLight * flashlight = new PointLight;
	flashlight->setDiffuseColor( Color( 255, 255, 255, 255 ));
	flashlight->setRange( 20 );
	flashlight->attachTo( player->getCamera() );
	
	while( true )
	{
		renderer->update();

		renderer->beginRender();

		if( keyboard.keyPressed( SDLK_ESCAPE ))
			break;

		world->update();
		
		SoundChannel::updateAll();

		renderer->endRender( true, false );
	};
	
	Texture::deleteAll();

	delete world;

	delete renderer;


	
	return 0;
}