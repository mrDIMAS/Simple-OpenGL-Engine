#pragma once

#include "Common.h"
#include "RigidBody.h"
#include "PickSphere.h"
#include "Scripting.h"

using namespace Dynamic;
/*
#include <unordered_map>

class Value
{
private:
	string val;
public:
	Value( )
	{
	};

	Value( const string & value )
	{
		setAsString( value );
	};

	Value( const float & value )
	{
		setAsNumber( value );
	};

	void setAsString( const string & value )
	{
		val = value;
	};

	void setAsNumber( const float & value )
	{
		char buf[ 32 ] = { 0 };	sprintf( buf, "%f", value );
		val = buf;
	};

	string getAsString( )
	{
		return val;
	};

	float getAsNumber( )
	{
		return atof( val.c_str() );
	};

	operator const char * ()
	{
		return val.c_str();
	};

	operator string () 
	{
		return val;
	};

	bool operator == ( const Value & value )
	{
		return val == value.val;
	};
};*/

class SceneNode
{
protected:
	btTransform globalTransform;
	btTransform localTransform;
	btVector3 scale;
	vector< SceneNode* > childs;
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
	//unordered_map< string, Value> properties;	
public:
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