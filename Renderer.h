#pragma once

#include "Common.h"
#include "DebugDrawer.h"
#include "SDL_syswm.h"
#include "Keyboard.h"
#include "Shader.h"

btDiscreteDynamicsWorld	* dynamicsWorld = 0;

unsigned int globalRenderFlags = GRF_USELIGHT;

PFNGLCREATESHADERPROC glCreateShader   = 0;
PFNGLDELETESHADERPROC glDeleteShader   = 0;
PFNGLSHADERSOURCEPROC glShaderSource   = 0;
PFNGLCOMPILESHADERPROC glCompileShader = 0;
PFNGLCREATEPROGRAMPROC glCreateProgram = 0;
PFNGLDELETEPROGRAMPROC glDeleteProgram = 0;
PFNGLATTACHSHADERPROC glAttachShader = 0;
PFNGLLINKPROGRAMPROC glLinkProgram = 0;
PFNGLUSEPROGRAMPROC glUseProgram = 0;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog = 0;
PFNGLACTIVETEXTUREPROC glActiveTexture = 0;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = 0;
PFNGLUNIFORM1IPROC glUniform1i = 0;

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

	void initSound()
	{
		pfSystemCreateLogFile( "ProjectF.log" );

		pfSystemEnableMessagesOutputToConsole();
		pfSystemEnableMessagesOutputToLogFile();

		pfSystemInit();
	};

	void updatePhysics()
	{
		dynamicsWorld->stepSimulation( 1.0f / 60.0f, -0 );
	};

	void initShaderSubsystem( )
	{
		glCreateShader  = (PFNGLCREATESHADERPROC)SDL_GL_GetProcAddress( "glCreateShader" );
		glDeleteShader  = (PFNGLDELETESHADERPROC)SDL_GL_GetProcAddress( "glDeleteShader" );
		glShaderSource  = (PFNGLSHADERSOURCEPROC)SDL_GL_GetProcAddress( "glShaderSource" );
		glCompileShader = (PFNGLCOMPILESHADERPROC)SDL_GL_GetProcAddress( "glCompileShader" );
		glCreateProgram = (PFNGLCREATEPROGRAMPROC)SDL_GL_GetProcAddress( "glCreateProgram" );
		glDeleteProgram = (PFNGLDELETEPROGRAMPROC)SDL_GL_GetProcAddress( "glDeleteProgram" );
		glAttachShader = (PFNGLATTACHSHADERPROC)SDL_GL_GetProcAddress( "glAttachShader" );
		glLinkProgram = (PFNGLLINKPROGRAMPROC)SDL_GL_GetProcAddress( "glLinkProgram" );
		glUseProgram = (PFNGLUSEPROGRAMPROC)SDL_GL_GetProcAddress( "glUseProgram" );
		glGetShaderInfoLog = (PFNGLGETSHADERINFOLOGPROC)SDL_GL_GetProcAddress( "glGetShaderInfoLog" );
		glActiveTexture = (PFNGLACTIVETEXTUREPROC)SDL_GL_GetProcAddress( "glActiveTexture" );
		glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)SDL_GL_GetProcAddress( "glGetUniformLocation" );
		glUniform1i = (PFNGLUNIFORM1IPROC)SDL_GL_GetProcAddress( "glUniform1i" );
	};
public:
	
	Renderer( uint w = 0, uint h = 0, bool fullscreen = false )
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

		if( w == 0 )
			w = info->current_w;
		if( h == 0 )
			h = info->current_h;

		uint flags = SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE;

		if( fullscreen )
			flags |= SDL_FULLSCREEN;

		SDL_Surface * back = SDL_SetVideoMode( w, h, 32, flags );

		if( !fullscreen )
		{
			SDL_SysWMinfo inf;		
			SDL_VERSION(&inf.version);
			SDL_GetWMInfo( &inf );		
			SetWindowPos( inf.window, 0, 0, 0, 0, 0, SWP_NOREPOSITION|SWP_NOZORDER|SWP_NOSIZE|SWP_NOACTIVATE );
		}

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

		initSound();

		initShaderSubsystem();

		g_default_shader = new Shader( "data/shaders/main.vs", "data/shaders/main.fs" );
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

		pfSystemUpdate();

		keyboard.update();

		SDL_GL_SwapBuffers();
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

		pfSystemDestroy();
	};
};