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

class Light
{
protected:
	unsigned char number;
	static unsigned char count;
	btVector3 position;
	btVector3 axis;
	Vector4 offset;
	float angle;

	static vector< Light* > lights;

	virtual void apply( )
	{
		glPushMatrix();

			glRotatef( angle, axis.x(), axis.y(), axis.z() );
			glTranslatef( position.x(), position.y(), position.z() );

			glLightfv( GL_LIGHT0 + number, GL_POSITION, offset );

		glPopMatrix();
	};
public:
	Light() : offset( Vector4( 0, 0, 0, 1 ) )
	{
		number = count;

		position = btVector3( 0, 0, 0 );
		angle = 0;
		axis = btVector3( 0, 1, 0 );

		count++;

		enable();
		setDiffuseColor( Color( 255, 255, 255, 255 ));
		setSpecularColor( Color( 255, 255, 255, 255 ));
		setAmbientColor( Color( 0, 0, 0, 0 ));
		setLinearAttenuation( 0 );
		setConstantAttenuation( 1 );
		setQuadraticAttenuation( 0 );

		lights.push_back( this );
	};

	btVector3 getPosition( )
	{
		btVector3 result = ( position + btVector3( offset.x, offset.y, offset.z )).rotate( axis, angle * DEG2RAD );

		return result;
	};

	void enable( )
	{
		glEnable( GL_LIGHT0 + number );
	};

	void disabled( )
	{
		glDisable( GL_LIGHT0 + number );
	};

	void setDiffuseColor( Color & color )
	{
		glLightfv( GL_LIGHT0 + number, GL_DIFFUSE,  color );
	};

	void setSpecularColor( Color & color )
	{
		glLightfv( GL_LIGHT0 + number, GL_SPECULAR,  color );
	};

	void setAmbientColor( Color & color )
	{
		glLightfv( GL_LIGHT0 + number, GL_AMBIENT,  color );
	};

	void setRotationAboutAxis( const btVector3 & axis, float angle )
	{
		this->axis  = axis;
		this->angle = angle;
	};

	void setOffset( Vector4 & offset )
	{
		this->offset = offset;
	};

	void setLinearAttenuation( float att )
	{
		glLightf( GL_LIGHT0 + number, GL_LINEAR_ATTENUATION, att );
	};

	void setConstantAttenuation( float att )
	{
		glLightf( GL_LIGHT0 + number, GL_CONSTANT_ATTENUATION, att );
	};

	void setQuadraticAttenuation( float att )
	{
		glLightf( GL_LIGHT0 + number, GL_QUADRATIC_ATTENUATION, att );
	};
	
	static void applyAll()
	{
		for( uint i = 0; i < lights.size(); i++ )
		{
			lights.at( i )->apply();
		};
	};
};

vector< Light* > Light::lights ;

unsigned char Light::count = 0;

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
	btTransform transform;

	vector< SceneNode* > childs;
	
	SceneNode * root;
	SceneNode * parent;

	virtual void render() = 0;

	bool rendered;

	friend class Scene;

	virtual void renderNodeAndChilds( )
	{
		glPushMatrix(); // Save matrix

		render();

		// Render childs relatively this node
		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();

		glPopMatrix(); // Restore matrix
	};

	friend class Camera;
	
public:
	 
	SceneNode( )
	{
		parent = 0;
		root   = 0;

		rendered = false;

		transform.setIdentity( );
	};

	SceneNode * getChild( uint num )
	{
		return childs.at( num );
	};

	virtual void attachTo( SceneNode * parent )
	{
		detachFromParent();

		if( !root )
			 root = parent;

		this->parent = parent;

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

			parent = 0; // mean's that this node can't be rendered, becase render starts from root node and goes down from hierarchy
		};
	};

	virtual void setPosition( const btVector3 & pos )
	{
		transform.setOrigin( pos );
	};

	virtual void setRotation( const btVector3 & euler )
	{
		transform.getBasis( ).setRotation( btQuaternion( euler.x() * DEG2RAD, euler.y() * DEG2RAD, euler.z() * DEG2RAD ));
	};

	virtual void turn( const btVector3 & euler )
	{
		transform.getBasis() *= btMatrix3x3( btQuaternion( euler.x() * DEG2RAD, euler.y() * DEG2RAD, euler.z() * DEG2RAD));
	};

	virtual void move( const btVector3 & pos )
	{
		//transform.setOrigin( ( transform.getOrigin() + pos ) * transform.getBasis() );
	}; 

	virtual void setScale( const btVector3 & scale )
	{		
		transform.setBasis( transform.getBasis().scaled( scale ));
	};
};

class Camera : public SceneNode
{
protected:
	virtual void render( )
	{
		glRotatef( xrot, 1.0, 0.0, 0.0 ); 
		glRotatef( yrot, 0.0, 1.0, 0.0 );  

		glTranslated( -x, -y, -z );
	};

	void renderNodeAndChilds( )
	{
		glMatrixMode( GL_MODELVIEW );

		// Render childs relatively this node

		glLoadIdentity();

		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();

		render(); // apply view matrix
	};	

public:

	float xrot, yrot;
	float x, y, z;

	static Camera * activeCamera;
	
	Camera()
	{
		xrot = yrot = x = y = z = 0;

		glMatrixMode( GL_PROJECTION );

		glLoadIdentity();

		gluPerspective( 70, 1.25, 0.1, 1024 );		

		activeCamera = this;
	};

	void update()
	{
		
		renderNodeAndChilds();

		Light::applyAll();
	};

	virtual void setPosition( const btVector3 & pos )
	{
		x = pos.x();
		y = pos.y();
		z = pos.z();
	};

	virtual void setRotation( const btVector3 & euler )
	{
		xrot = euler.x();
		yrot = euler.y();
	};

	virtual void turn( const btVector3 & euler )
	{
		xrot += euler.x();
		yrot += euler.y();

		if( xrot > 360 )
			xrot = -360;
		if( xrot < -360 )
			xrot = 360;

		if( yrot > 360 )
			yrot = -360;
		if( yrot < -360 )
			yrot = 360;
	};

	virtual void move( const btVector3 & speed )
	{
		float xrotrad, yrotrad;

		yrotrad = ( yrot * DEG2RAD );
		xrotrad = ( xrot * DEG2RAD );

		x +=  speed.z() * sinf( yrotrad ) ;
		y += -speed.z() * sinf( xrotrad ) ;   
		z += -speed.z() * cosf( yrotrad ) ;

		yrotrad = ( ( yrot + 90 ) * DEG2RAD );

		x +=  speed.x() * sinf( yrotrad ) ;
		z += -speed.x() * cosf( yrotrad ) ;
	}; 

	/*
	void pitch( float speed )
	{
		xrot += speed;

		if( xrot > 360 )
			xrot = -360;
		if( xrot < -360 )
			xrot = 360;
	};

	void yaw( float speed )
	{
		yrot += speed;

		if( yrot > 360 )
			yrot = -360;
		if( yrot < -360 )
			yrot = 360;
	};*/
	/*
	void move( float speed, float strafe = 0 )
	{
		float xrotrad, yrotrad;

		yrotrad = ( yrot * DEG2RAD );
		xrotrad = ( xrot * DEG2RAD );

		x +=  speed * sinf( yrotrad ) ;
		y += -speed * sinf( xrotrad ) ;   
		z += -speed * cosf( yrotrad ) ;

		yrotrad = ( ( yrot + 90 ) * DEG2RAD );

		x +=  strafe * sinf( yrotrad ) ;
		z += -strafe * cosf( yrotrad ) ;
	};*/
};

Camera * Camera::activeCamera = 0;

class Mesh : public SceneNode
{
protected:
	vector< Surface* > surfaces;

	string name;

	virtual void render( )
	{
		float matrix[ 16 ];

		transform.getOpenGLMatrix( matrix );

		glMultMatrixf( matrix );	

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
		body->setWorldTransform( transform );

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
		body->setWorldTransform( transform );

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
		body->setWorldTransform( transform );

		dynamicsWorld->addRigidBody( body );
	};

	virtual void render()
	{
		transform = body->getWorldTransform(); // replace current transformation by rigid body transformation

		Mesh::render();
	};
};
 
void beginRender()
{
	glClearColor( 1, 1, 1, 1 );

	glClear( GL_COLOR_BUFFER_BIT |	GL_DEPTH_BUFFER_BIT );

	if( Camera::activeCamera )
		Camera::activeCamera->update();
};

void endRender()
{
	SDL_GL_SwapBuffers();
};


void controlCamera()
{
	static int lastMX = 0, lastMY = 0;

	Camera * camera = Camera::activeCamera;

	uint8_t * keyboard = SDL_GetKeyState( 0 );
		
	float speed = 0;
	float strafe = 0;

	if( keyboard[ SDLK_w ] )
		speed = 0.2;
	if( keyboard[ SDLK_s ] )
		speed = -0.2;
	if( keyboard[ SDLK_a ] )
		strafe = -0.2;
	if( keyboard[ SDLK_d ] )
		strafe =  0.2;

	//camera->move( speed, strafe );
	camera->move( btVector3( strafe, 0, speed ));

	int mx, my;
		
	SDL_GetMouseState( &mx, &my );

	int mxs = mx - lastMX;
	int mys = my - lastMY;

	//camera->pitch( mys * 0.2 );
	//camera->yaw( mxs * 0.2 );
	camera->turn( btVector3( mys * 0.2, mxs * 0.2, 0 ));

	SDL_WarpMouse( 200, 200 );

	lastMX = 200;
	lastMY = 200;	
};

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
		//assignSphereBody( 2, 1 );
	};

	void update()
	{
		/*
		static int lastMX = 0, lastMY = 0;

		
		Camera * camera = Camera::activeCamera;

		btVector3 look = btVector3( 

		uint8_t * keyboard = SDL_GetKeyState( 0 );
		
		float speed = 0;
		float strafe = 0;

		if( keyboard[ SDLK_w ] )
			speed = 0.2;
		if( keyboard[ SDLK_s ] )
			speed = -0.2;
		if( keyboard[ SDLK_a ] )
			strafe = -0.2;
		if( keyboard[ SDLK_d ] )
			strafe =  0.2;

		camera->move( speed, strafe );

		int mx, my;
		
		SDL_GetMouseState( &mx, &my );

		int mxs = mx - lastMX;
		int mys = my - lastMY;

		camera->pitch( mys * 0.2 );
		camera->yaw( mxs * 0.2 );

		SDL_WarpMouse( 200, 200 );

		lastMX = 200;
		lastMY = 200;	*/
	};
};

int main(int argc, char *argv[])
{	
	initGL();
	initPhysics();

	Light lit1;
	lit1.setOffset( Vector4( -100, 10, 0, 1 ));
	lit1.setDiffuseColor( Color( 255, 255, 255, 255 ));
	
	Camera camera;

	Scene scene;

	scene.loadO3D( "level.o3d" );
	/*
	for( uint i = 0; i < level.size(); i++ )
	{
		if( level.at( i )->getName() == "Plane01" )
			level.at( i )->assignTriMeshBody( );
		else
			level.at( i )->assignConvexBody( 1 );
	}
	*/

	scene.getChild( 2 )->attachTo( scene.getChild( 1 ) );

	while( true )
	{
		processEvents();

		beginRender();
		
		controlCamera();

		scene.renderNodeAndChilds();
		
		scene.getChild( 1 )->turn( btVector3( 0, 0.2, 0 ));
		scene.getChild( 1 )->getChild( 0 )->turn( btVector3( 0.2, 0, 0 ));
		//level.at( 1 )->turn( btVector3( 0, 0.002, 0 ));
		// level.at( 2 )->turn( btVector3( -0.05, 0 , 0 ));

		//dynamicsWorld->debugDrawWorld();

		updatePhysics();

		endRender();
	};

	return 0;
}