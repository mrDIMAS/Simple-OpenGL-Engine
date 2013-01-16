#pragma once

#include "Common.h"
#include "RigidBody.h"

class SceneNode
{
protected:
	btTransform globalTransform;
	btTransform localTransform;
	btVector3 scale;
	vector< SceneNode* > childs;

	RigidBody * body;

	SceneNode * parent;

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
	uint getChildCount()
	{
		return childs.size();
	};
	RigidBody * getRigidBody()
	{
		return body;
	};
};