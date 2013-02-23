#pragma once

#include "Common.h"
#include "DebugDrawer.h"
#include "SDL_syswm.h"

btDiscreteDynamicsWorld	* dynamicsWorld = 0;

class Renderer
{
private:
	btDefaultCollisionConfiguration			* defaultCollision;
	btCollisionDispatcher					* collisionDispatcher;
	btBroadphaseInterface					* broadphase;
	btSequentialImpulseConstraintSolver		* solver;	

	void initPhysics()
	{
		btVector3 worldMin( -10000, -10000, -10000 );
		btVector3 worldMax(  10000,  10000,  10000 );

		defaultCollision	= new btDefaultCollisionConfiguration();

		collisionDispatcher = new btCollisionDispatcher( defaultCollision );
		broadphase			= new btAxisSweep3( worldMin, worldMax );
		solver				= new btSequentialImpulseConstraintSolver();
		dynamicsWorld		= new btDiscreteDynamicsWorld( collisionDispatcher, broadphase, solver, defaultCollision );


		dynamicsWorld->setDebugDrawer( new MyDebugDrawer );

		dynamicsWorld->setGravity( btVector3( 0.f, -25.81f, 0.f ));	
	};

	void updatePhysics()
	{
		dynamicsWorld->stepSimulation( 1.0f / 60.0f, 10 );
	};

public:
	
	Renderer( bool fullscreen = false )
	{	
		SDL_Init( SDL_INIT_EVERYTHING );
		
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
 
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1 );
		SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4 );
	
		SDL_GL_SetAttribute(SDL_GL_SWAP_CONTROL, 1);

		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		if( !fullscreen )
		{
			SDL_Surface * back = SDL_SetVideoMode( info->current_w, info->current_h, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE );
			SDL_SysWMinfo inf;		
			SDL_VERSION(&inf.version);
			SDL_GetWMInfo( &inf );		
			SetWindowPos( inf.window, 0, 0, 0, 0, 0, SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE );
		}
		else
			SDL_Surface * back = SDL_SetVideoMode( info->current_w, info->current_h, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN );

		glEnable	  ( GL_MULTISAMPLE );

		glEnable	  ( GL_DEPTH_TEST  );
		glDepthFunc   ( GL_LEQUAL );

		glEnable	  ( GL_TEXTURE_2D  );
		//glEnable	  ( GL_NORMALIZE );
		
		glClearColor  ( 0, 0, 0, 1 );
		glClearDepth  ( 1.0f );
		
		glHint        ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	

		//glEnable( GL_BLEND );
		glBlendFunc( GL_SRC_ALPHA, GL_ONE );  

		glEnable      ( GL_LIGHTING );
		glShadeModel  ( GL_SMOOTH );
		glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
		glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
		glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );		
		
		glEnable	  ( GL_CULL_FACE );
		glCullFace	  ( GL_BACK );

		glEnable	  ( GL_ALPHA_TEST );
		glAlphaFunc   ( GL_GREATER, 0.8 );

		initPhysics();
	}

	void beginRender()
	{
		glClear( GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT );
	};	

	void endRender( bool renderPhysics = true, bool renderPhysicsWireframeDebug = true )
	{
		if( renderPhysics )
			updatePhysics();

		if( renderPhysicsWireframeDebug )
			dynamicsWorld->debugDrawWorld();

		SDL_GL_SwapBuffers();
	};

	void beginRender2D( )
	{

	};

	void endRender2D()
	{

	};

	void update()
	{
		SDL_Event event;

		while( SDL_PollEvent( &event ) ) 
		{
			switch( event.type ) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_QUIT:
				break;
			}
		}
	};

	~Renderer()
	{
		if( dynamicsWorld->getDebugDrawer() )
			delete dynamicsWorld->getDebugDrawer();

		delete dynamicsWorld;
	
		delete solver;
	
		delete broadphase;
	
		delete collisionDispatcher;

		delete defaultCollision;

		SDL_Quit( );
	};
};