#pragma once

#pragma warning( disable : 4244 )
#pragma warning( disable : 4305 )

#include "SDL.h"
#include "SDL_main.h"
#include "SDL_opengl.h"
#include "SDL_keyboard.h"

#include <gl\glaux.h>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <vld.h>
#include <map>

#include "ProjectF/ProjectF.h"
#pragma comment( lib, "ProjectF/ProjectF.lib" )

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "Color.h"
#include "Vector4.h"

#pragma comment( lib, "sdlmain" )
#pragma comment( lib, "sdl" )
#pragma comment( lib, "opengl32" )
#pragma comment( lib, "glu32" )
#pragma comment( lib, "glaux" )
#pragma comment( lib, "vld" )

#ifndef _DEBUG

	#pragma comment( lib, "BulletCollision" )
	#pragma comment( lib, "BulletDynamics" )
	#pragma comment( lib, "LinearMath" )

#else

	#pragma comment( lib, "BulletCollision_debug" )
	#pragma comment( lib, "BulletDynamics_debug" )
	#pragma comment( lib, "LinearMath_debug" )

#endif

class Keyboard;
class Mouse;
class World;
class Player;

extern btDiscreteDynamicsWorld * dynamicsWorld;
extern Keyboard keyboard;
extern Mouse mouse;
extern World * world;
extern Player * player;
extern unsigned int globalRenderFlags;

enum
{
	GRF_USELIGHT = 1,
};

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char byte;

#define DEG2RAD ( 3.141592654f / 180.0f )
#define RAD2DEG ( 180.0f / 3.141592654f )

using namespace std;

static int random( int min, int max )
{
	return rand() * ( max - min ) / RAND_MAX + min;
};

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLDELETESHADERPROC glDeleteShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;

extern PFNGLACTIVETEXTUREPROC glActiveTexture;
extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLUNIFORM1IPROC glUniform1i;

class Shader;

extern Shader * g_default_shader;

