#pragma once

#include "Common.h"
#include "RigidBody.h"
#include "PickSphere.h"

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
	
	void applyChangesRelatively();
public:	 
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
	virtual void setUserPointer( void * pointer )
	{
		userPointer = pointer;
	};
	virtual void * getUserPointer( )
	{
		return userPointer;
	};
	string & getName( );
	void setName( string &newName )
	{
		name=newName;
	};
	SceneNode * getNodeByRay( Ray * ray )
	{
		SceneNode * result = 0;

		bool intersects = false;

		if( pickObj )
			intersects = pickObj->intersectByRay( ray );

		if( !intersects )
		{
			for( uint i = 0; i < childs.size(); i++ )
			{
				result = childs.at( i )->getNodeByRay( ray );

				if( result )
					break;
			};
		}
		else
		{
			result = this;
		};

		return result;
	};

	void setPickObject( PickObject * pickObject )
	{
		pickObj = pickObject;
		pickObj->pos = &globalTransform.getOrigin();
	};
	uint getChildCount()
	{
		return childs.size();
	};
	RigidBody * getRigidBody()
	{
		return body;
	};
};