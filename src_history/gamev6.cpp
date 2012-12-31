#include "SDL.h"
#include "SDL_main.h"
#include "SDL_opengl.h"
#include "SDL_keyboard.h"

#include <gl\glaux.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#pragma comment( lib, "sdlmain" )
#pragma comment( lib, "sdl" )
#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32" )
#pragma comment( lib, "glaux" )

#ifndef _DEBUG

#pragma comment( lib, "BulletCollision" )
#pragma comment( lib, "BulletDynamics" )
#pragma comment( lib, "LinearMath" )

#else

#pragma comment( lib, "BulletCollision_debug" )
#pragma comment( lib, "BulletDynamics_debug" )
#pragma comment( lib, "LinearMath_debug" )

#endif

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;

#define DEG2RAD ( 3.141592654f / 180.0f )
#define RAD2DEG ( 180.0f / 3.141592654f )

btDefaultCollisionConfiguration			* defaultCollision;
btCollisionDispatcher					* collisionDispatcher;
btBroadphaseInterface					* broadphase;
btSequentialImpulseConstraintSolver		* solver;
btDiscreteDynamicsWorld					* dynamicsWorld;

using namespace std;

void setupGLLight()
{
	glLightModeli( GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE );
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
	glLightModeli( GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR );

	glEnable      ( GL_LIGHTING );
};

class MyDebugDrawer : public btIDebugDraw
{
public:
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
	{
		glBegin( GL_LINES );
		glColor4f( color.x(), color.y(), color.z(), 1 );
		glVertex3f( from.x(), from.y(), from.z() );
		glColor4f( color.x(), color.y(), color.z(), 1 );
		glVertex3f( to.x(), to.y(), to.z() );
		glEnd();
	};

	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{

	};

	void reportErrorWarning(const char* warningString)
	{

	};

	void draw3dText(const btVector3& location,const char* textString)
	{

	}
	
	void setDebugMode(int debugMode)
	{

	};
	
	int	getDebugMode() const 
	{
		return 1;
	};
};
void initGL()
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

	bool fs = 0;

	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	if( !fs )
		SDL_Surface * back = SDL_SetVideoMode( 800, 600, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE );
	else
		SDL_Surface * back = SDL_SetVideoMode( info->current_w, info->current_h, 32, SDL_DOUBLEBUF | SDL_OPENGL | SDL_HWSURFACE | SDL_FULLSCREEN );

	glEnable	  ( GL_MULTISAMPLE );
	glEnable	  ( GL_DEPTH_TEST  );
	glEnable	  ( GL_TEXTURE_2D  );
	glEnable	  ( GL_NORMALIZE );
	glShadeModel  ( GL_SMOOTH );
	glClearColor  ( 0, 0, 0, 1 );
	glClearDepth  ( 1.0f );
    glDepthFunc   ( GL_LEQUAL );
	glHint        ( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	
	glBlendFunc   ( GL_SRC_ALPHA, GL_ONE );  
	setupGLLight  ();

	glEnable	  ( GL_CULL_FACE );
	glCullFace	  ( GL_BACK );
};

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

	dynamicsWorld->setGravity( btVector3( 0.f, -9.81f, 0.f ));	
};

void updatePhysics()
{
	dynamicsWorld->stepSimulation( 1.0f / 60.0f, 1 );
};

class Color
{
private:
	union
	{
		struct
		{
			float r, g, b, a;
		};

		float c[ 4 ];
	};
public:
	Color( unsigned char red, unsigned char green, unsigned char blue, unsigned char alpha )
	{
		r = red / 255.0f;
		g = green / 255.0f;
		b = blue / 255.0f;
		a = alpha / 255.0f;
	};
	
	operator float * ()
	{
		return c;
	};

	float getRed()
	{
		return r;
	};

	float getGreen()
	{
		return g;
	};

	float getBlue()
	{
		return b;
	};
};

class Vector4
{
public:
	union
	{
		struct
		{
			float x, y, z, w;
		};

		float components[ 4 ];
	};

	Vector4( float x, float y, float z, float w )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	};

	operator float * ()
	{
		return components;
	};
};



class Material
{
public:
	Material()
	{

	};

	void bind()
	{

	};
};

class Vertex
{
public:
	float x,y,z;
	float nx,ny,nz;
	float tx,ty;

	Vertex(  )
	{
		x = y = z = nx = ny = nz = tx = ty = 0;
	};

	Vertex( float x, float y, float z, float nx, float ny, float nz, float tx, float ty )
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->nx = nx;
		this->ny = ny;
		this->nz = nz;
		this->tx = tx;
		this->ty = ty;
	};
};

static unsigned char uTGAcompare[12] = {0,0,2,0,0,0,0,0,0,0,0};

struct TGAHeader
{
	unsigned char Header[12];
};

struct TGA
{
	unsigned char Header[6];
	unsigned int bytesPerPixel;
	unsigned int imageSize;
	unsigned int type;
	unsigned int Height;
	unsigned int Width;
	unsigned int BPP;
};

class Texture
{
public:
	uint id;
	uint width;
	uint height;
	uint bitsPerPixel;
	uint format;

	void loadTGA( const char * filename )
	{
		TGAHeader targaHeader;
		TGA targa;

		FILE * file = fopen( filename, "rb" );

		fread( &targaHeader, sizeof(TGAHeader), 1, file );

		if( !memcmp( uTGAcompare, &targaHeader, sizeof( targaHeader )))
		{
			fread( targa.Header, sizeof( targa.Header ), 1, file );

			width         = targa.Header[1] * 256 + targa.Header[0];
			height        = targa.Header[3] * 256 + targa.Header[2];
			bitsPerPixel  = targa.Header[4];
	
			format = bitsPerPixel == 24 ? GL_RGB : GL_RGBA;

			uint size = width * height * ( bitsPerPixel / 8 );

			unsigned char * imageData = new unsigned char[ size ];

			fread( imageData, 1, size, file );

			for( uint cswap = 0; cswap < size; cswap += ( bitsPerPixel / 8 ))
			{
				imageData[cswap] ^= imageData[cswap + 2] ^= imageData[cswap] ^= imageData[cswap + 2];
			};

			fclose( file );

			glGenTextures( 1, &id );

			glBindTexture( GL_TEXTURE_2D, id );

			glTexImage2D( GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, imageData );

			glBindTexture( GL_TEXTURE_2D, 0 );

			delete [] imageData;
		}
	};

	void loadBMP( const char * filename )
	{
		AUX_RGBImageRec * texture1;

		texture1 = auxDIBImageLoadA( filename );

		glGenTextures( 1, &id );

		glBindTexture( GL_TEXTURE_2D, id );

		glTexImage2D( GL_TEXTURE_2D, 0, 3, texture1->sizeX, texture1->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, texture1->data );

		glBindTexture( GL_TEXTURE_2D, 0 );
	};

	void bind()
	{
		glBindTexture( GL_TEXTURE_2D, id );

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	};
};

class Surface
{
protected:
	vector< Vertex* > vertices;
	vector< uint > indices;
	Texture * texture;

public:
	Surface()
	{
		texture = 0;
	};

	void addVertex( Vertex * v )
	{
		vertices.push_back( v );
	};

	Vertex * getVertex( uint num )
	{
		return vertices[ num ];
	};

	uint getVertexCount()
	{
		return vertices.size();
	};
	
	void addIndex( uint index )
	{
		indices.push_back( index );
	};

	uint getIndex( uint num )
	{
		return indices[ num ];
	};

	uint getIndexCount()
	{
		return indices.size();
	};

	void setTexture( Texture * texture )
	{
		this->texture = texture;
	}

	void render()
	{
		if( texture )
			texture->bind();

		glBegin( GL_TRIANGLES );

		for( uint i = 0; i < indices.size(); i += 3 )
		{
			for( uint indexNum = 0; indexNum < 3; indexNum++ )
			{
				Vertex * v = vertices[ indices[ i + indexNum ]];

				glTexCoord3f( v->tx, v->ty, 1 );
				glNormal3f( v->nx, v->ny, v->nz );
				glVertex3f( v->x, v->y, v->z );
			}
		};		

		glEnd();

		glBindTexture( GL_TEXTURE_2D, 0 );
	};
};


class SceneNode
{
protected:
	btTransform globalTransform;
	btTransform localTransform;
	btVector3 scale;
	vector< SceneNode* > childs;
	
	SceneNode * parent;

	virtual void render() = 0;

	friend class Camera;
	friend class Scene;

	virtual void renderNodeAndChilds( )
	{
		glPushMatrix(); // Save matrix

		applyChangesRelatively();
		
		globalTransform.setBasis( globalTransform.getBasis().scaled( scale ));

		render();

		glPopMatrix(); // Restore matrix

		// Render childs
		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();	
	};	

	void applyChangesRelatively()
	{
		if( parent )
			globalTransform = parent->globalTransform * localTransform ;
		else
			globalTransform = localTransform;
	};
	
public:
	 
	SceneNode( )
	{
		parent = 0;
		scale = btVector3( 1, 1, 1 );
		localTransform.setIdentity( );
		globalTransform.setIdentity( );
	};

	SceneNode * getChild( uint num )
	{
		return childs.at( num );
	};

	void applyTransform()
	{
		float matrix[ 16 ];

		globalTransform.getOpenGLMatrix( matrix );

		glMultMatrixf( matrix );	
	}

	virtual void attachTo( SceneNode * parent, bool saveRelativePositionAndOrientation = true )
	{
		detachFromParent();
			
		this->parent = parent;

		if( !saveRelativePositionAndOrientation )
		{
			setPosition( btVector3( 0, 0, 0 ));
			setRotation( btVector3( 0, 0, 0 ));
		}			

		parent->childs.push_back( this );
	};
	 
	virtual void detachFromParent( )
	{
		if( parent )
		{
			for( uint i = 0; i < parent->childs.size(); i++ )
			{
				if( parent->childs.at( i ) == this )
				{
					parent->childs.erase( parent->childs.begin() + i );

					break;
				};
			};

			 // mean's that this node can't be rendered, becase render starts from root node and goes down from hierarchy
			parent = 0;
		};
	};

	virtual void setPosition( const btVector3 & pos )
	{
		localTransform.setOrigin( pos );

		applyChangesRelatively();
	};
	
	virtual btVector3 getRelativePosition( bool global = false )
	{
		return localTransform.getOrigin();
	};

	virtual btVector3 getGlobalPosition( bool global = false )
	{
		return globalTransform.getOrigin();
	};

	virtual void setRotation( const btVector3 & euler )
	{
		localTransform.getBasis( ).setRotation( btQuaternion( -euler.y() * DEG2RAD, -euler.x() * DEG2RAD, -euler.z() * DEG2RAD ));

		applyChangesRelatively();
	};

	virtual void turn( const btVector3 & euler )
	{
		localTransform.getBasis() *= btMatrix3x3( btQuaternion( -euler.y() * DEG2RAD, -euler.x() * DEG2RAD, -euler.z() * DEG2RAD));

		applyChangesRelatively();
	};
	
	virtual btVector3 getRotation( bool global = false )
	{		
		btVector3 out;

		return out;
	};
	
	virtual void move( const btVector3 & speed )
	{
		localTransform.setOrigin( localTransform.getOrigin() - localTransform.getBasis().getColumn( 0 ) * speed.x() + localTransform.getBasis().getColumn( 2 ) * speed.z() );		

		applyChangesRelatively();
	}; 

	virtual void setScale( const btVector3 & scale )
	{		
		this->scale = scale;
	};
};

class Light : public SceneNode
{
protected:
	GLenum id;

	static unsigned char count;
	
public:
	Light() 
	{
		id = GL_LIGHT0 + count;

		count++;

		enable();
		setDiffuseColor( Color( 255, 255, 255, 255 ));
		setSpecularColor( Color( 255, 255, 255, 255 ));
		setAmbientColor( Color( 0, 0, 0, 0 ));
		setLinearAttenuation( 0 );
		setConstantAttenuation( 1 );
		setQuadraticAttenuation( 0 );
		setDirection( Vector4( 0, 0, 0, 0 ));
	};

	void enable( )
	{
		glEnable( id );
	};

	void disabled( )
	{
		glDisable( id );
	};

	void setDiffuseColor( Color & color )
	{
		glLightfv( id, GL_DIFFUSE,  color );
	};

	void setDirection( Vector4 dir  )
	{
		glLightfv( id, GL_SPOT_DIRECTION, dir );
	};

	void setSpecularColor( Color & color )
	{
		glLightfv( id, GL_SPECULAR,  color );
	};

	void setAmbientColor( Color & color )
	{
		glLightfv( id, GL_AMBIENT,  color );
	};

	void setLinearAttenuation( float att )
	{
		glLightf( id, GL_LINEAR_ATTENUATION, att );
	};

	void setConstantAttenuation( float att )
	{
		glLightf( id, GL_CONSTANT_ATTENUATION, att );
	};

	void setQuadraticAttenuation( float att )
	{
		glLightf( id, GL_QUADRATIC_ATTENUATION, att );
	};
};

class DirectionalLight : public Light
{
public:
	virtual void render()
	{
		float vpos[ 4 ] = { 0, 0, 1, 0 };

		applyTransform();

		glLightfv( id, GL_POSITION, vpos );		
	};
};

class PointLight : public Light
{
public:
	virtual void render()
	{
		float vpos[ 4 ] = { 0, 0, 0, 1 };
	
		applyTransform();

		glLightfv( id, GL_POSITION, vpos );		
	};
};

unsigned char Light::count = 0;

class Camera : public SceneNode
{
protected:
	virtual void render( )
	{
		btVector3 eye  = globalTransform.getOrigin();
		btVector3 look = eye + globalTransform.getBasis().getColumn( 2 );
		btVector3 up   = globalTransform.getBasis().getColumn( 1 );

		gluLookAt( eye.x(),  eye.y(),  eye.z(), 
			      look.x(), look.y(), look.z(), 
				    up.x(),   up.y(),   up.z() );
	};
	
	void renderNodeAndChilds( )
	{
		glMatrixMode( GL_MODELVIEW );
				
		glLoadIdentity();
		
		render(); // apply view matrix
				
		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();			
	};	

public:

	static Camera * activeCamera;
	
	Camera()
	{
		glMatrixMode( GL_PROJECTION );

		glLoadIdentity();

		gluPerspective( 70, 1.25, 0.1, 1024 );		

		activeCamera = this;
	};

	void setClearColor( Color color )
	{
		glClearColor( color.getRed(), color.getGreen(), color.getBlue(), 1 );
	};
};

Camera * Camera::activeCamera = 0;

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
};

class RigidMesh : public Mesh
{
public:
	btRigidBody * body;
	btCollisionShape * collisionShape;
	btMotionState * motionState;

	RigidMesh( )
	{
		body = 0;
	};

	void initMotionState( )
	{
		motionState = new btDefaultMotionState( );
	};

	void setPosition( const btVector3 & pos )
	{
		body->getWorldTransform().setOrigin( pos );
	};

	void setRotation( const btVector3 & euler )
	{
		body->getWorldTransform( ).getBasis( ).setRotation( btQuaternion( euler.x(), euler.y(), euler.z() ));
	};

	void setScale( const btVector3 & scale )
	{		
		collisionShape->setLocalScaling( scale );

		body->getWorldTransform().setBasis( body->getWorldTransform().getBasis().scaled( scale ));
	};

	void assignSphereBody( float rad, float mass )
	{
		btVector3 inertia( 0.0f, 0.0f, 0.0f );

		btSphereShape * sphere = new btSphereShape( rad );

		collisionShape = sphere;

		initMotionState();

		collisionShape->calculateLocalInertia( mass, inertia );

		body = new btRigidBody( mass, motionState, collisionShape, inertia );
	
		//Установка текущей трансформации
		//ody->setWorldTransform( transform );

		dynamicsWorld->addRigidBody( body );
	};

	void assignConvexBody( float mass )
	{
		if( surfaces.size() == 0 )
			return;
			
		btVector3 inertia( 0.0f, 0.0f, 0.0f );

		btConvexHullShape * convex = new btConvexHullShape( );

		collisionShape = convex;

		for( uint i = 0; i < surfaces.size(); i++ )
		{
			Surface * surface = surfaces[ i ];

			for( uint vc = 0; vc < surface->getVertexCount(); vc++ )
			{
				Vertex * vertex = surface->getVertex( vc );

				convex->addPoint( btVector3( vertex->x, vertex->y, vertex->z ));
			};
		};		

		initMotionState();

		collisionShape->calculateLocalInertia( mass, inertia );

		body = new btRigidBody( mass, motionState, collisionShape, inertia );
	
		//Установка текущей трансформации
		//body->setWorldTransform( transform );

		dynamicsWorld->addRigidBody( body );
	};

	void assignTriMeshBody( )
	{
		if( surfaces.size() == 0 )
			return;
			
		btVector3 inertia( 0.0f, 0.0f, 0.0f );

		btTriangleMesh * mesh = new btTriangleMesh( true, false );
		
		for( uint i = 0; i < surfaces.size(); i++ )
		{
			Surface * surface = surfaces[ i ];

			for( uint ic = 0; ic < surface->getIndexCount(); ic+=3 )
			{
				Vertex * a = surface->getVertex( surface->getIndex( ic + 0 ) );
				Vertex * b = surface->getVertex( surface->getIndex( ic + 1 ) );
				Vertex * c = surface->getVertex( surface->getIndex( ic + 2 ) );
				
				mesh->addTriangle( btVector3( a->x, a->y, a->z ), btVector3( b->x, b->y, b->z), btVector3( c->x, c->y, c->z) );
			};
		};		

		btBvhTriangleMeshShape * trimesh = new btBvhTriangleMeshShape( mesh, true, true );

		collisionShape = trimesh;

		initMotionState();

		body = new btRigidBody( 0, motionState, collisionShape, inertia );
		
		//Установка текущей трансформации
		//body->setWorldTransform( transform );

		dynamicsWorld->addRigidBody( body );
	};

	virtual void render()
	{
		//transform = body->getWorldTransform(); // replace current transformation by rigid body transformation

		Mesh::render();
	};
};
 
void beginRender()
{
	glClear( GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT );
};

void endRender()
{
	SDL_GL_SwapBuffers();
};

class Mouse
{
private:
	void saveCurrentCoord()
	{
		SDL_GetMouseState( &lx, &ly );
	};
public:
	int lx, ly;
	int sx, sy;

	Mouse( )
	{
		sx = sy = 0;
		saveCurrentCoord();
	};

	void moveTo( ushort x, ushort y )
	{
		SDL_WarpMouse( x, y );

		lx = x; ly = y;
	};

	void update()
	{
		sx = getX() - lx;
		sy = getY() - ly;

		saveCurrentCoord();
	};

	int getXSpeed()
	{
		return sx;
	};

	int getYSpeed()
	{
		return sy;
	};

	int getX()
	{
		int x, y;

		SDL_GetMouseState( &x, &y );

		return x;
	};

	int getY()
	{
		int x, y;

		SDL_GetMouseState( &x, &y );

		return y;
	};
} mouse;

class Keyboard
{
public:
	Keyboard( )
	{

	};

	bool keyPressed( uint8_t key )
	{
		uint8_t * keyboard = SDL_GetKeyState( 0 );

		return keyboard[ key ];
	};
} keyboard;

class Quad : public Mesh
{
public:
	Quad( )
	{
		Surface * surf = new Surface;

		surf->addVertex( new Vertex( -1, -1, 0, 0, 0, 0, 0, 0 ));
		surf->addVertex( new Vertex(  1, -1, 0, 0, 0, 0, 1, 0 ));
		surf->addVertex( new Vertex(  1,  1, 0, 0, 0, 0, 1, 1 ));
		surf->addVertex( new Vertex( -1,  1, 0, 0, 0, 0, 0, 1 ));

		surf->addIndex( 0 ); surf->addIndex( 1 ); surf->addIndex( 2 );
		surf->addIndex( 0 ); surf->addIndex( 2 ); surf->addIndex( 3 );

		addSurface( surf );
	}
};

static void processEvents( void )
{
    SDL_Event event;

    while( SDL_PollEvent( &event ) ) 
	{
        switch( event.type ) 
		{
        case SDL_KEYDOWN:
			if( event.key.keysym.sym == SDLK_ESCAPE )
			{
				SDL_Quit();
				exit( 0 );
			}
            break;
        case SDL_QUIT:

			SDL_Quit();

			exit( 0 );

            break;
        }
    }
}


// provides tree-like hierarchy

class Scene : public SceneNode // :) feeling like a ROOT! :)
{
public:
	Scene( )
	{
		
	};

	void loadO3D( const char * filename )
	{
		FILE * file = fopen( filename, "rb" );

		ulong numObjects;
		fread( &numObjects, sizeof( numObjects ), 1, file );

		for( ulong objNum = 0; objNum < numObjects; objNum++ )
		{
			Mesh * mesh = new Mesh;

			Surface * surface = new Surface;

			ulong vertexCount;
			ulong faceCount;		

			fread( &faceCount, sizeof( faceCount ), 1, file );		

			float px, py, pz, rx, ry, rz;

			fread( &px, sizeof( px ), 1, file );
			fread( &py, sizeof( py ), 1, file );
			fread( &pz, sizeof( pz ), 1, file );

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

			// read diffuse texture name
			string difTexture = "";

			fread( &symbol, sizeof( symbol ), 1, file );		
		
			while( symbol )
			{
				difTexture.push_back( symbol );

				fread( &symbol, sizeof( symbol ), 1, file );
			};

			for( ulong i = 0; i < faceCount; i++ )
			{
				for( uint j = 0; j < 3; j++ )
				{
					float x = 0, y = 0, z = 0, nx = 0, ny = 0, nz = 0, tx = 0, ty = 0;

					fread( &x, 1, sizeof( x ), file );
					fread( &y, 1, sizeof( y ), file );
					fread( &z, 1, sizeof( z ), file );
				
					fread( &nx, 1, sizeof( nx ), file );
					fread( &ny, 1, sizeof( ny ), file );
					fread( &nz, 1, sizeof( nz ), file );

					fread( &tx, 1, sizeof( tx ), file );
					fread( &ty, 1, sizeof( ty ), file );

					surface->addVertex( new Vertex( x, y, z, nx, ny, nz, tx, ty ));
				}

				for( uint j = 0; j < 3; j++ )
				{
					ulong index; fread( &index, 1, sizeof( index ), file );
	
					surface->addIndex( index );
				}
			};

			mesh->addSurface( surface );

			if( difTexture.size() )
			{
				Texture * texture =	new Texture();
				texture->loadTGA( difTexture.c_str() );

				mesh->setTexture( texture );
			}

			mesh->setPosition( btVector3( px, py, pz ) );
			mesh->setRotation( btVector3( rx, ry, rz ) );

			mesh->attachTo( this );
		}

		fclose( file );
	}

	void render () 
	{
		// render text here
	}

	void renderNodeAndChilds( )
	{
		// Render childs relatively this node
		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();
	};
};

class Player : public Mesh
{
public:
	Player()
	{
		
	};

	void update()
	{

	};
};

void drawRightHandedGizmoXYZWhereZLookToward()
{
/*			| +y
			|
			|
			|____________ +x
		   /
          /
		 / +z
*/
	glMatrixMode( GL_MODELVIEW );

	glDisable( GL_LIGHTING );

	glBegin( GL_LINES );

		// x
		glColor4f( 1, 0, 0, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 100, 0, 0 );
		
		// y
		glColor4f( 0, 1, 0, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 100, 0 );

		// z
		glColor4f( 0, 0, 1, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 0, 100 );

	glEnd();

	glEnable( GL_LIGHTING );
};

void ethalonViewTransform()
{
	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();
		
	gluLookAt( 0, 100, 100, 0, 0, 0, 0, 1, 0 );

	glScalef( 1, 1, -1 );
};

int main(int argc, char *argv[])
{	
	initGL();
	initPhysics();

	PointLight lit1;
	lit1.setDiffuseColor( Color( 255, 255, 255, 255 ));
	lit1.setLinearAttenuation( 0.01 );

	Camera camera;
	int numLights;
	glGetIntegerv(GL_MAX_LIGHTS, &numLights);

	Scene scene;

	scene.loadO3D( "level.o3d" );

	SDL_WarpMouse( 200, 200 );

	Mesh * cube4   = (Mesh*)scene.getChild( 5 );
	Mesh * cube3   = (Mesh*)scene.getChild( 4 );
	Mesh * cube2   = (Mesh*)scene.getChild( 3 );
	Mesh * cube    = (Mesh*)scene.getChild( 2 );
	Mesh * pyramid = (Mesh*)scene.getChild( 1 );

	pyramid->setScale( btVector3( 0.1, 0.1, 0.1 ));

	cube4->attachTo( cube3, 1 );
	cube3->attachTo( cube2, 1 );
	
	
	camera.attachTo( cube, 0 ); camera.setPosition( btVector3( 0, 40, 0 ));
	
	lit1.attachTo( &camera );

	//pyramid->attachTo( &camera, 0 ); pyramid->setPosition( btVector3( 0, 0, 25 ));

	while( true )
	{
		processEvents();

		beginRender();
				
		//controlCamera();		

		scene.renderNodeAndChilds();

		drawRightHandedGizmoXYZWhereZLookToward();
		
		float speed = 0;
		float strafe = 0;

		if( keyboard.keyPressed( SDLK_w ) )
			speed = 0.2;
		if( keyboard.keyPressed( SDLK_s ) )
			speed = -0.2;
		if( keyboard.keyPressed( SDLK_a ) )
			strafe = -0.2;
		if( keyboard.keyPressed( SDLK_d ) )
			strafe =  0.2;

		cube->move( btVector3( strafe, 0, speed ));

		camera.turn( btVector3( -mouse.getYSpeed() * 0.2, 0, 0 ));
		cube->turn( btVector3( 0, mouse.getXSpeed() * 0.2, 0 ));
		cube3->turn( btVector3( 1, 0, 0) );
		cube4->turn( btVector3( 0, 0, 1 ) );
		pyramid->turn( btVector3( 1, 0, 0 ));

		mouse.moveTo( 200, 200 );
		
		mouse.update();

		cube2->turn( btVector3( 0, 1, 0));

		//dynamicsWorld->debugDrawWorld();

		updatePhysics();

		endRender();
	};

	return 0;
}