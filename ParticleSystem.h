#pragma once

#include "SceneNode.h"

enum
{
	PS_RANDOMIZE_SPEED			= 1,
	PS_RANDOMIZE_LIFETIME		= 2,
	PS_RANDOMIZE_POSITION		= 4,
	PS_RANDOMIZE_ORIENTATION	= 8,
	PS_INCREASE_SIZE			= 16,
	PS_DECREASE_SIZE			= 32,
	PS_BRIGHT_PARTICLES			= 64,
};

class Particle
{
public:
	btVector3 pos;
	btVector3 force;
	btVector3 posDelta;

	float forceMult;
	float sizeMult;
	float gravMult;

	float w, h;
	long time;
	long timeDelta;

	long fullTime;

	Particle( const btVector3 & p, long lifeTime, float width, float height )
	{
		resurrect( p, lifeTime, width, height );

		timeDelta	= 0;
		force		= btVector3( 0, 0, 0 );
		posDelta	= btVector3( 0, 0, 0 );
		forceMult   = 1;
		sizeMult	= 1;
		gravMult	= 1;

		fullTime = lifeTime;
	};

	void randomForce( )
	{
		force = btVector3( random( -100, 100 ) * 0.01, random( -100, 100 ) * 0.01, random( -100, 100 ) * 0.01 );
	};

	void randomPosition( )
	{
		posDelta = btVector3( random( -100, 100 ) * 0.01, random( -100, 100 ) * 0.01, random( -100, 100 ) * 0.01 );
	};
	
	void randomTimeDelta( )
	{
		timeDelta = random( -30, 30 );
	};

	void setForceMult( float fm )
	{
		forceMult = fm;
	};

	void setSizeMult( float sm )
	{
		sizeMult = sm;
	}

	void setGravMult( float gm )
	{
		gravMult = gm;
	}

	void decreaseSize( )
	{
		w -= fullTime * 0.0001;
		h -= fullTime * 0.0001;

		if( w < 0 )
			w = 0;

		if( h < 0 )
			h = 0;
	};

	void renderAndUpdate( )
	{
		btVector3 realPos = pos + posDelta;

		glTexCoord2f( 0, 0 );
		glVertex3f(   realPos.x(), realPos.y(), realPos.z() );

		glTexCoord2f( 0, 1 );
		glVertex3f(   realPos.x(), realPos.y() + h * sizeMult,  realPos.z() );

		glTexCoord2f( 1, 1 );
		glVertex3f(   realPos.x() + w * sizeMult, realPos.y() + h * sizeMult,  realPos.z() );

		glTexCoord2f( 1, 0 );
		glVertex3f(   realPos.x() + w * sizeMult, realPos.y(),  realPos.z() );

		time--;

		btVector3 realForce = gravMult * btVector3( 0, -1, 0 ) + forceMult * btVector3(  force.x(), force.y(), force.z() );

		pos += realForce;		
	};

	bool isDead( )
	{
		return time <= 0;
	};

	void resurrect( const btVector3 & p, long lifeTime, float width, float height )
	{
		w		= width;
		h		= height;
		time	= lifeTime + timeDelta;
		pos		= p;	
	};
};

class ParticleSystem : public SceneNode
{
private:
	btVector3 speed;
	Texture * texture;

	vector< Particle* > particles;
	float pw;
	float ph;
	long plifetime;		
	long flags;

public:
	
	void setFlags( uint flag )
	{
		flags |= flag;
	};

	void skipFlags( )
	{
		flags = 0;
	};

	void skipFlag( uint flag )
	{
		flags &= ~flag;
	};

	void setForceMult( float fm )
	{
		for( uint i = 0; i < particles.size(); i++ )
		{
			Particle * p = particles.at( i );

			p->setForceMult( fm );
		}
	};

	void setGravMult( float gm )
	{
		for( uint i = 0; i < particles.size(); i++ )
		{
			Particle * p = particles.at( i );

			p->setGravMult( gm );
		};
	}

	void setSizeMult( float sm )
	{
		for( uint i = 0; i < particles.size(); i++ )
		{
			Particle * p = particles.at( i );

			p->setSizeMult( sm );
		}
	}

	ParticleSystem( const char * texFile, uint count, float pw, float ph, long plifetime = 300 )
	{
		texture = Texture::loadTGA( texFile, false );
		
		this->pw			= pw;
		this->ph			= ph;
		this->plifetime		= plifetime;
		this->flags			= 0;

		for( uint i = 0; i < count; i++ )
		{
			particles.push_back( new Particle( getGlobalPosition( true ), plifetime, pw, ph ));
		};
	};

	~ParticleSystem( )
	{
		for( uint i = 0; i < particles.size(); i++ )
		{
			delete particles.at( i );
		};
	};


	void render()
	{
		if( !visible )
			return;

		btVector3 position = globalTransform.getOrigin();

		texture->bind();

		glDisable( GL_CULL_FACE );	

		if( flags & PS_BRIGHT_PARTICLES )
			glDisable( GL_LIGHTING );

		for( uint i = 0; i < particles.size(); i++ )
		{
			Particle * p = particles.at( i );

			glPushMatrix();

				if( flags & PS_RANDOMIZE_ORIENTATION )
					glRotatef( random( -180, 180 ) * 0.1, 0, 1, 0 );
				
				if( flags & PS_DECREASE_SIZE )
					p->decreaseSize();

				glBegin( GL_QUADS );

					p->renderAndUpdate();

				glEnd();

			glPopMatrix();

			if( p->isDead() )
			{
				p->resurrect( position, plifetime, pw, ph );

				if( flags & PS_RANDOMIZE_LIFETIME )
					p->randomTimeDelta();

				if( flags & PS_RANDOMIZE_POSITION )
					p->randomPosition();

				if( flags & PS_RANDOMIZE_SPEED )
					p->randomForce();
			};			
		};	

		if( flags & PS_BRIGHT_PARTICLES )
			glEnable( GL_LIGHTING );

		glDisable( GL_CULL_FACE );
	};
};