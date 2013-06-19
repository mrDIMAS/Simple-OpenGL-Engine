#pragma once

#include "Common.h"
#include "RigidBody.h"
#include "PickSphere.h"
#include "Scripting.h"
#include "Shader.h"

using namespace Dynamic;

class SceneNode
{
protected:
	
	btTransform localTransform;
	btVector3 scale;
	vector< SceneNode* > childs;
	Shader * shader;
	RigidBody * body;
	PickObject * pickObj;
	SceneNode * parent;
	void * userPointer;
	string name;
	virtual void render() = 0;
	friend class Camera;
	friend class Scene;
	bool visible;
	void applyChangesRelatively();
	map<string,vector<Sound>> sounds;
	void syncSounds( )
	{
		float x = globalTransform.getOrigin().x();
		float y = globalTransform.getOrigin().y();
		float z = globalTransform.getOrigin().z();

		for( auto group = sounds.begin(); group != sounds.end(); group++ )
		{
			vector< Sound > & gr = group->second;

			for( uint i = 0; i < gr.size(); i++ )
			{
				pfSetSoundPosition( gr.at( i ), x, y, z );
			};
		};
	};
public:
	btTransform globalTransform;
	Dynamic::Parser::ValueMap properties;	
	static vector<SceneNode*> nodes;
	SceneNode( );	
	virtual ~SceneNode( );
	SceneNode * getChild( uint num );
	void applyTransform();
	virtual void attachTo( SceneNode * parent, bool saveRelativePositionAndOrientation = true );
	virtual void detachFromParent( );
	virtual void setPosition( const btVector3 & pos );
	virtual void setGlobalPosition( const btVector3 & pos );
	virtual btVector3 getRelativePosition( bool global = false );
	virtual btVector3 getGlobalPosition( bool global = false );
	virtual void setRotation( const btVector3 & euler );
	virtual void turn( const btVector3 & euler );
	virtual btVector3 getRotation();
	virtual btVector3 getGlobalRotation();
	virtual void move( const btVector3 & speed );
	virtual void setScale( const btVector3 & scale );
	virtual void setRigidBody( RigidBody * body );	
	virtual void renderNodeAndChilds( );
	string & getName( );
	virtual void setUserPointer( void * pointer );
	virtual void * getUserPointer( );
	float getDistanceTo( SceneNode * node );
	void setName( string &newName );
	void hide();
	void show( ) ;

	void addSoundToGroup( string groupName, Sound snd )
	{
		sounds[ groupName ].push_back( snd );
	};
	
	void playSoundFromGroup( string groupName, uint n, bool oneshot = true )
	{
		pfPlaySound( sounds[ groupName ].at( n ), oneshot );
	};

	void playRandomSoundFromGroup( string groupName, bool oneshot = true )
	{
		pfPlaySound( sounds[ groupName ].at( rand() % sounds[ groupName ].size() ), oneshot );
	};

	SceneNode * findByNameProp( string & name )
	{
		
		SceneNode * result = 0;

		if( hasProperty( "name" ))
		{
			if( properties[ "name" ].getStr() == name )
				result = this;
		}
		else
		{
			for( uint i = 0; i < childs.size(); i++ )
			{
				result = childs.at( i )->findByNameProp( name );

				if( result )
					break;
			};
		};

		return result;
	};
	
	/*
	static SceneNode * findByName( string name )
	{
		for( uint i = 0; i < nodes.size(); i++ )
		{
			SceneNode * node = nodes.at( i );

			if( node->hasProperty( "name
		};
	};*/

	bool hasProperty( string name )
	{
		auto it = properties.find( name );

		return it != properties.end();
	};

	void addProperty( const string & name, const Value & val )
	{
		properties[ name ] = val;
	};

	Value & getProperty( const string & name ) 
	{
		return (*properties.find( name )).second;
	};

	void setProperty( const string & name, const Value & val )
	{
		(*properties.find( name )).second = val;
	};

	SceneNode * getNodeByRay( Ray * ray );
	void setPickObject( PickObject * pickObject );
	uint getChildCount();
	RigidBody * getRigidBody();
};