#pragma once

#include "Common.h"
#include "SceneNode.h"

class SoundChannel : public SceneNode
{
private:
	uint channel;

	friend class Sound;

	SoundChannel( uint sndData )
	{
		alGenSources( 1, &channel );

		alSourcei( channel, AL_BUFFER, sndData );

		alSourcePlay( channel );

		channels.push_back( this );
	};

	~SoundChannel( )
	{
		this->detachFromParent();

		alSourceStop( channel );

		alDeleteSources( 1, &channel );
	};
public:

	static vector< SoundChannel* > channels;

	void render( )
	{
		btVector3 position	= globalTransform.getOrigin();				 float fpos[ 3 ]  = { position.x(), position.y(), position.z() };
		btVector3 look		= globalTransform.getBasis().getColumn( 0 ).normalize(); float flook[ 3 ] = { look.x(), look.y(), look.z() };		

		alSourcefv( channel, AL_POSITION, fpos );
		alSourcefv( channel, AL_DIRECTION, flook );
	};

	static void updateAll()
	{
		for( uint i = 0; i < channels.size(); i++ )
		{
			SoundChannel * channel = channels.at( i );

			int state = 0;

			alGetSourcei( channel->channel, AL_SOURCE_STATE, &state );

			if( state == AL_STOPPED )
			{
				delete channel;

				channels.erase( channels.begin() + i );
			};
		};
	};
};

vector< SoundChannel* > SoundChannel::channels;


class Sound
{
public:
	int format;
	int freq;
	uint buffer;

	friend class SoundChannel;

	Sound( const char * filename )
	{
		FILE * file = fopen( filename, "rb" );

		OggVorbis_File * vorbisFile = new OggVorbis_File;

		ov_open_callbacks( file, vorbisFile, 0, -1, OV_CALLBACKS_DEFAULT );

		vorbis_comment * vorbisComments = ov_comment( vorbisFile, -1 );
		vorbis_info	   * vorbisInfo = ov_info( vorbisFile, -1 );

		if( vorbisInfo->channels == 1 )
		{
			format = AL_FORMAT_MONO16;
			freq = vorbisInfo->rate;

			int totalcharsReaden = 0;
			int channel			 = 0;

			uint size =  ov_pcm_total( vorbisFile, -1 ) * sizeof( short );
			char * data = new char[ size ];
		
			while( true )
			{
				int charsReaden = ov_read( vorbisFile, (char*)( data + totalcharsReaden), size - totalcharsReaden, 0, 2, 1, &channel );
						
				if( charsReaden <= 0 )
					break;		

				totalcharsReaden += charsReaden;
			};

			alGenBuffers( 1, &buffer );
			
			alBufferData( buffer, format, data, size, freq );

			delete data;			
		}

		ov_clear( vorbisFile );

		delete vorbisFile;

		fclose( file );
	};

	~Sound()
	{
		alDeleteBuffers( 1, &buffer );
	};

	SoundChannel * play( SceneNode * emitter )
	{
		SoundChannel * channel = new SoundChannel( buffer );

		channel->attachTo( emitter );

		return channel;
	};
};

