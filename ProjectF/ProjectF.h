///////////////////////////////////////////////////////////
//														 //
//			   P	R	O	J	E	C	T	F			 //
//														 //
//		    ProjectF - is a PROJECTtion of Feelings		 //
//														 //
//			      S O U N D   E N G I N E				 //
//														 //
//	     All rights reserved mr.DIMAS 2012 - 2013 (C)	 //
//														 //
///////////////////////////////////////////////////////////

/*
				ATTENTION!

		DEFAULT COORDINATE SYSTEM


				 +Y
				^
				|
				|
				|
				|
				|
				|
				|
				-----------------------> +X
			   /
			  /
			 /
	        /
		   /
		  / +Z
*/

#ifndef _PROJECT_F_
#define _PROJECT_F_

#ifdef _EXPORT_
	#define API _declspec( dllexport )	
#else
	#define API
#endif

typedef long long Sound;
typedef long long SoundData;
typedef long long Group;
typedef long long Zone;
typedef long long Obstacle;
typedef unsigned int Filter;

typedef enum 
{
	//GENERIC
	PRESET_NONE = -1,
	PRESET_GENERIC,
	PRESET_PADDEDCELL,
	PRESET_ROOM,
	PRESET_BATHROOM,
	PRESET_LIVINGROOM,
	PRESET_STONEROOM,
	PRESET_AUDITORIUM,
	PRESET_CONCERTHALL,
	PRESET_CAVE,
	PRESET_ARENA,
	PRESET_HANGAR,
	PRESET_CARPETTEDHALLWAY,
	PRESET_HALLWAY,
	PRESET_STONECORRIDOR,
	PRESET_ALLEY,
	PRESET_FOREST,
	PRESET_CITY,
	PRESET_MOUNTAINS,
	PRESET_QUARRY,
	PRESET_PLAIN,
	PRESET_PARKINGLOT,
	PRESET_SEWERPIPE,
	PRESET_UNDERWATER,
	PRESET_DRUGGED,
	PRESET_DIZZY,
	PRESET_PSYCHOTIC,

	//CASTLEPRESETS
	PRESET_CASTLE_SMALLROOM,
	PRESET_CASTLE_SHORTPASSAGE,
	PRESET_CASTLE_MEDIUMROOM,
	PRESET_CASTLE_LONGPASSAGE,
	PRESET_CASTLE_LARGEROOM,
	PRESET_CASTLE_HALL,
	PRESET_CASTLE_CUPBOARD,
	PRESET_CASTLE_COURTYARD,
	PRESET_CASTLE_ALCOVE,


	//FACTORYPRESETS
	PRESET_FACTORY_ALCOVE,
	PRESET_FACTORY_SHORTPASSAGE,
	PRESET_FACTORY_MEDIUMROOM,
	PRESET_FACTORY_LONGPASSAGE,
	PRESET_FACTORY_LARGEROOM,
	PRESET_FACTORY_HALL,
	PRESET_FACTORY_CUPBOARD,
	PRESET_FACTORY_COURTYARD,
	PRESET_FACTORY_SMALLROOM,

	//ICEPALACEPRESETS
	PRESET_ICEPALACE_ALCOVE,
	PRESET_ICEPALACE_SHORTPASSAGE,
	PRESET_ICEPALACE_MEDIUMROOM,
	PRESET_ICEPALACE_LONGPASSAGE,
	PRESET_ICEPALACE_LARGEROOM,
	PRESET_ICEPALACE_HALL,
	PRESET_ICEPALACE_CUPBOARD,
	PRESET_ICEPALACE_COURTYARD,
	PRESET_ICEPALACE_SMALLROOM,

	//SPACESTATIONPRESETS
	PRESET_SPACESTATION_ALCOVE,
	PRESET_SPACESTATION_MEDIUMROOM,
	PRESET_SPACESTATION_SHORTPASSAGE,
	PRESET_SPACESTATION_LONGPASSAGE,
	PRESET_SPACESTATION_LARGEROOM,
	PRESET_SPACESTATION_HALL,
	PRESET_SPACESTATION_CUPBOARD,
	PRESET_SPACESTATION_SMALLROOM,

	//WOODENGALLEONPRESETS
	PRESET_WOODEN_ALCOVE,
	PRESET_WOODEN_SHORTPASSAGE,
	PRESET_WOODEN_MEDIUMROOM,
	PRESET_WOODEN_LONGPASSAGE,
	PRESET_WOODEN_LARGEROOM,
	PRESET_WOODEN_HALL,
	PRESET_WOODEN_CUPBOARD,
	PRESET_WOODEN_SMALLROOM,
	PRESET_WOODEN_COURTYARD,

	//SPORTSPRESETS
	PRESET_SPORT_EMPTYSTADIUM,
	PRESET_SPORT_SQUASHCOURT,
	PRESET_SPORT_SMALLSWIMMINGPOOL,
	PRESET_SPORT_LARGESWIMMINGPOOL,
	PRESET_SPORT_GYMNASIUM,
	PRESET_SPORT_FULLSTADIUM,
	PRESET_SPORT_STADIUMTANNOY,

	//PREFABPRESETS
	PRESET_PREFAB_WORKSHOP,
	PRESET_PREFAB_SCHOOLROOM,
	PRESET_PREFAB_PRACTISEROOM,
	PRESET_PREFAB_OUTHOUSE,
	PRESET_PREFAB_CARAVAN,

	//DOMEANDPIPEPRESETS
	PRESET_DOME_TOMB,
	PRESET_PIPE_SMALL,
	PRESET_DOME_SAINTPAULS,
	PRESET_PIPE_LONGTHIN,
	PRESET_PIPE_LARGE,
	PRESET_PIPE_RESONANT,

	//OUTDOORSPRESETS
	PRESET_OUTDOORS_BACKYARD,
	PRESET_OUTDOORS_ROLLINGPLAINS,
	PRESET_OUTDOORS_DEEPCANYON,
	PRESET_OUTDOORS_CREEK,
	PRESET_OUTDOORS_VALLEY,

	//MOODPRESETS
	PRESET_MOOD_HEAVEN,
	PRESET_MOOD_HELL,
	PRESET_MOOD_MEMORY,

	//DRIVINGSIMULATIONPRESETS
	PRESET_DRIVING_COMMENTATOR,
	PRESET_DRIVING_PITGARAGE,
	PRESET_DRIVING_INCAR_RACER,
	PRESET_DRIVING_INCAR_SPORTS,
	PRESET_DRIVING_INCAR_LUXURY,
	PRESET_DRIVING_FULLGRANDSTAND,
	PRESET_DRIVING_EMPTYGRANDSTAND,
	PRESET_DRIVING_TUNNEL,

	//CITYPRESETS
	PRESET_CITY_STREETS,
	PRESET_CITY_SUBWAY,
	PRESET_CITY_MUSEUM,
	PRESET_CITY_LIBRARY,
	PRESET_CITY_UNDERPASS,
	PRESET_CITY_ABANDONED,

	//MISCROOMS
	PRESET_DUSTYROOM,
	PRESET_CHAPEL,
	PRESET_SMALLWATERROOM,

} Preset;

///////////////////////////////////////////////////////////
//Possible error states
///////////////////////////////////////////////////////////
enum
{
	PF_NO_ERROR = -666,
	PF_FILE_DOES_NOT_EXIST,
	PF_DEVICE_NOT_FOUND,
	PF_OUT_OF_MEMORY,
	PF_UNSUPPORTED_FILE_TYPE,
	PF_BAD_PCM_DATA,
	PF_BAD_DATA_IDENTIFIER,
	PF_BAD_SOUND_IDENTIFIER,
	PF_BAD_ZONE_IDENTIFIER,
	PF_BAD_GROUP_IDENTIFIER,
	PF_UNABLE_TO_CREATE_LOG_FILE,
};

///////////////////////////////////////////////////////////
//Possible atmosphere types
///////////////////////////////////////////////////////////
#define PF_ATMOSPHERE_WET_AIR		( 5.12 )
#define	PF_ATMOSPHERE_DRY_AIR		( 1.60 )
#define	PF_ATMOSPHERE_NORMAL_AIR	( 0.10 )

///////////////////////////////////////////////////////////
//Possible non rotated coordinate systems
///////////////////////////////////////////////////////////
enum 
{
	PF_CS_Y_UP_X_RIGHT_Z_OUT_OF_SCREEN, // DEFAULT
/*
				 +Y
				^
				|
				|
				|
				|
				----------> +X
			   /	        
		      /
		     / +Z
*/
	PF_CS_Y_UP_X_RIGHT_Z_IN_SCREEN,
/*
			+Y
			^
			|     Z+
			|  /
			| /
			|/
			----------> +X

*/

};

///////////////////////////////////////////////////////////
//System
///////////////////////////////////////////////////////////

//Initialization\destruction
extern API bool pfSystemInit();
extern API void pfSystemDestroy();

//Function's helpful to determinate position of sounds. 
extern API void pfSystemSetDebugRenderFunction( void (*func)( float x, float y, float z ));
extern API void pfSystemSetDebugRenderFunctionLine( void (*func)( float xs, float ys, float zs, float xe, float ye, float ze ));
extern API void pfSystemDebugRender();
extern API void pfSystemSetCoordinateSystem( int cs );

//Update
extern API void pfSystemUpdate();

//Basic parameters
extern API void pfSystemSetSpeedOfSound( float speed ); //default is 343 m\s
extern API unsigned int pfSystemGetTotalMemoryUsed( );

//Error handling
extern API int pfSystemGetLastError();
extern API void pfSystemEnableMessagesOutputToConsole();
extern API void pfSystemDisableMessagesOutputToConsole();

//Log file
extern API void pfSystemCreateLogFile( const char * filename );
extern API void pfSystemEnableMessagesOutputToLogFile();
extern API void pfSystemDisableMessagesOutputToLogFile();

///////////////////////////////////////////////////////////
//Sound data
///////////////////////////////////////////////////////////
extern API SoundData pfDataLoad( const char * file, bool streaming = false );
extern API SoundData pfDataRead( char * bytes, int size, int freq, int bits );
extern API void pfDataDestroy( SoundData data );
extern API void pfDataDestroyAllSources();

///////////////////////////////////////////////////////////
//Sound
///////////////////////////////////////////////////////////

//Sound Data playback
extern API Sound pfCreateSound( SoundData data, bool is3D = false, bool deleteAfterPlayed = false );

//State
extern API void pfPlaySound( Sound sound, bool oneshot = true );
extern API void pfStopSound( Sound sound );
extern API void pfPauseSound( Sound sound );
extern API bool pfIsSoundPlaying( Sound sound );

//Basic parameters

//Volume
extern API void pfSetSoundVolume( Sound sound, float volume ); //from 0 to 1
extern API float pfGetSoundVolume( Sound sound );
extern API void pfSetMinMaxVolume( Sound sound, float min, float max ); //from 0 to 1
extern API void pfGetMinMaxVolume( Sound sound, float*  min, float * max );

//Distance
extern API float pfGetMaxDistance( Sound sound );
extern API void pfSetMaxDistance( Sound sound, float max );

//Pitch
extern API void pfSetSoundPitch( Sound sound, float pitch ); //from 0 to 1
extern API void pfChangeSoundPitch( Sound sound, float deltaPitch );
extern API float pfGetSoundPitch( Sound sound );

//Sound cone parameters
extern API void pfSetSoundConeParameters( Sound sound, float innerAngle, float outerAngle, float outerGain );
extern API void pfGetSoundConeParameters( Sound sound, float * innerAngle, float * outerAngle, float * outerGain );
extern API void pfSetSoundConeDirection( Sound sound, float x, float y, float z );
extern API void pfGetSoundConeDirection( Sound sound, float * x, float * y, float * z );

//Filtering
extern API void pfSoundAttachFilter( Sound sound, Filter filter );

//Position
extern API void pfSetSoundPosition( Sound sound, float x, float y, float z );
extern API void pfGetSoundPosition( Sound sound, float * v );

//Destruction
extern API void pfFreeSound( Sound sound );
extern API void pfFreeAllSounds( );

///////////////////////////////////////////////////////////
//Group
///////////////////////////////////////////////////////////
extern API Group pfGroupCreate( );

extern API void pfGroupAddSound( Group group, Sound sound );
extern API void pfGroupPlay( Group group, bool oneshot = true );
extern API void pfGroupPlayRandomSound( Group group, bool oneshot = true );
extern API void pfGroupStop( Group group );
extern API unsigned int pfGroupGetSize( Group group );
extern API Sound pfGroupGetSound( Group group, unsigned int soundN );
extern API void pfGroupSetPosition( Group group, float * pos );
extern API void pfGroupSetPosition( Group group, float x, float y, float z );
extern API void pfGroupSetVolume( Group group, float volume );
extern API void pfGroupSetMinMaxVolume( Group group, float min, float max ); //from 0 to 1
extern API void pfGroupSetPitch( Group group, float pitch ); //from 0 to 1

///////////////////////////////////////////////////////////
//Listener
///////////////////////////////////////////////////////////

//Position
extern API void pfSetListenerPosition( float x, float y, float z );
extern API void pfGetListenerPosition( float * v );

//Orientation
extern API void pfSetListenerOrientation( float * look, float * up );

//Basic parameters
extern API void pfSetListenerDopplerFactor( float factor ); //from 0 to 1
extern API void pfSetListenerRolloffFactor( float factor ); //from 0 to 1
extern API void pfSetListenerReferenceFactor( float factor ); //from 0 to 1

extern API void pfSetListenerSetEAXPreset( Preset preset );

///////////////////////////////////////////////////////////
// Zone
///////////////////////////////////////////////////////////
extern API Zone pfZoneCreateAABB( Preset preset, float * min, float * max ); // AABB Zone
extern API Zone pfZoneCreateAABBFromVertices( Preset preset, int count, float * v[ 3 ] ); // AABB Zone from vertices
extern API Zone pfZoneCreateSphere( Preset preset, float * position, float radius ); // Sphere Zone
extern API void pfZoneSetPosition( Zone zone, float * position );
extern API void pfZoneAABBSetMinMax( Zone zone, float * min, float * max );
extern API void pfZoneAABBSetMin( Zone zone, float * min );
extern API void pfZoneAABBSetMax( Zone zone, float * max );
extern API void pfZoneActivate( Zone zone );
extern API void pfZoneDeactivate( Zone zone );
extern API bool pfZoneIsActive( Zone zone );
extern API void pfZoneAddPoint( Zone zone, float * point ); // recomputes AABB min&max or Sphere radius
extern API void pfZoneSetAtmosphere( Zone zone, float atmosphere );

///////////////////////////////////////////////////////////
// Filter
///////////////////////////////////////////////////////////
extern API Filter pfFilterCreateLowpass( float gain, float hfGain );
extern API void pfFilterSetHFGain( Filter filter, float hfGain );
extern API void pfFilterSetGain( Filter filter, float gain );

///////////////////////////////////////////////////////////
// Obstacle
///////////////////////////////////////////////////////////
extern API Obstacle pfObstacleCreateAABB( float * min, float * max );

#endif