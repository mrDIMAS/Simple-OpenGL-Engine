#pragma once

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
#include "al.h"
#include "alc.h"
#include "thirdparty\Ogg\ogg.h"
#include "thirdparty\Vorbis\vorbisfile.h"

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
#pragma comment( lib, "openal32" )
#pragma comment( lib, "thirdparty/ogg/libogg_static.lib" )
#pragma comment( lib, "thirdparty/vorbis/libvorbis_static.lib" )
#pragma comment( lib, "thirdparty/vorbis/libvorbisfile_static.lib" )

#ifndef _DEBUG

	#pragma comment( lib, "BulletCollision" )
	#pragma comment( lib, "BulletDynamics" )
	#pragma comment( lib, "LinearMath" )

#else

	#pragma comment( lib, "BulletCollision_debug" )
	#pragma comment( lib, "BulletDynamics_debug" )
	#pragma comment( lib, "LinearMath_debug" )

#endif


extern btDiscreteDynamicsWorld * dynamicsWorld;

typedef unsigned int uint;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef unsigned char byte;

#define DEG2RAD ( 3.141592654f / 180.0f )
#define RAD2DEG ( 180.0f / 3.141592654f )

using namespace std;
