#include "SceneNode.h"

vector<SceneNode*> SceneNode::nodes;

void SceneNode::renderNodeAndChilds( )
{

	glPushMatrix(); // Save matrix

	if( body )
	{
		body->getPhysBody()->activate( true );

		globalTransform = body->getTransform();
	}
	else
	{
		applyChangesRelatively();
		
		globalTransform.setBasis( globalTransform.getBasis().scaled( scale ));
	}		
	
	if( visible )
		render();

	glPopMatrix(); // Restore matrix

	// Render childs
	for( uint i = 0; i < childs.size(); i++ )
		childs.at( i )->renderNodeAndChilds();	
};	

void SceneNode::applyChangesRelatively()
{
	if( parent )
		globalTransform = parent->globalTransform * localTransform ;
	else
		globalTransform = localTransform;
};

void SceneNode::setUserPointer( void * pointer )
{
	userPointer = pointer;
};

void * SceneNode::getUserPointer( )
{
	return userPointer;
};

float SceneNode::getDistanceTo( SceneNode * node )
{
	return globalTransform.getOrigin().distance( node->globalTransform.getOrigin() );
};
	
void SceneNode::setName( string &newName )
{
	name=newName;
};

void SceneNode::hide()
{
	visible = false;
};

void SceneNode::show( ) 
{
	visible = true;
};

SceneNode * SceneNode::getNodeByRay( Ray * ray )
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

void SceneNode::setPickObject( PickObject * pickObject )
{
	pickObj = pickObject;
	pickObj->pos = &globalTransform.getOrigin();
};

uint SceneNode::getChildCount()
{
	return childs.size();
};

RigidBody * SceneNode::getRigidBody()
{
	return body;
};

SceneNode::SceneNode( )
{
	parent = 0;
	body   = 0;
	pickObj= 0;
	userPointer = 0;
	show();
	scale = btVector3( 1, 1, 1 );
	localTransform.setIdentity( );
	globalTransform.setIdentity( );
	nodes.push_back( this );
};

SceneNode::~SceneNode( )
{
	for( uint i = 0; i < childs.size(); i++ )
	{
		delete childs.at( i );
	};

	auto it = find( nodes.begin(), nodes.end(), this );

	if( it != nodes.end())
		nodes.erase( it );

	if( body )
		delete body;
};

string & SceneNode::getName( )
{
	return name;
};

SceneNode * SceneNode::getChild( uint num )
{
	return childs.at( num );
};

void SceneNode::applyTransform()
{
	float matrix[ 16 ];

	globalTransform.getOpenGLMatrix( matrix );

	glMultMatrixf( matrix );	
}

void SceneNode::attachTo( SceneNode * parent, bool saveRelativePositionAndOrientation )
{
	detachFromParent();
			
	this->parent = parent;

	if( !saveRelativePositionAndOrientation )
	{
		setPosition( btVector3( 0, 0, 0 ));
		setRotation( btVector3( 0, 0, 0 ));
	}			

	parent->childs.push_back( this );
};
	 
void SceneNode::detachFromParent( )
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

			// mean's that this node can't be rendered, becase render starts from root node and goes down from hierarchy
		parent = 0;
	};
};

void SceneNode::setPosition( const btVector3 & pos )
{
	localTransform.setOrigin( pos );

	applyChangesRelatively();

	if( body )
		body->setTransform( globalTransform );
};

void SceneNode::setGlobalPosition( const btVector3 & pos )
{
	if( parent )
		localTransform.setOrigin( pos - parent->globalTransform.getOrigin() );
	else
		localTransform.setOrigin( pos );

	applyChangesRelatively();

	if( body )
		body->setTransform( globalTransform );
};
	
btVector3 SceneNode::getRelativePosition( bool global )
{
	return localTransform.getOrigin();
};

btVector3 SceneNode::getGlobalPosition( bool global )
{
	return globalTransform.getOrigin();
};

void SceneNode::setRotation( const btVector3 & euler )
{
	localTransform.getBasis( ).setRotation( btQuaternion( -euler.y() * DEG2RAD, -euler.x() * DEG2RAD, -euler.z() * DEG2RAD ));

	applyChangesRelatively();

	if( body )
		body->setTransform( globalTransform );
};

void SceneNode::turn( const btVector3 & euler )
{
	localTransform.getBasis() *= btMatrix3x3( btQuaternion( -euler.y() * DEG2RAD, -euler.x() * DEG2RAD, -euler.z() * DEG2RAD));

	applyChangesRelatively();
	
	if( body )
		body->getTransform().setBasis( globalTransform.getBasis());
};
	
btVector3 SceneNode::getRotation( )
{		
	float y, p, r;

	localTransform.getBasis( ).getEulerYPR( y, p, r );

	return btVector3( p * RAD2DEG, y * RAD2DEG, r * RAD2DEG );
};
	

btVector3 SceneNode::getGlobalRotation()
{
	float y, p, r;
	
	globalTransform.getBasis( ).getEulerYPR( y, p, r );

	return btVector3( p * RAD2DEG, y * RAD2DEG, r * RAD2DEG );
};

void SceneNode::move( const btVector3 & speed )
{
	if( body )
	{
		btVector3 speedf = -(body->getTransform().getBasis().getColumn( 0 ).normalize()) * speed.x() + (body->getTransform().getBasis().getColumn( 2 ).normalize()) * speed.z();

		body->setLinearVelocity( btVector3( 0, body->getLinearVelocity().y(), 0 ) + speedf );
	}
	else
	{
		localTransform.setOrigin( localTransform.getOrigin() - localTransform.getBasis().getColumn( 0 ) * speed.x() + localTransform.getBasis().getColumn( 2 ) * speed.z() );		

		applyChangesRelatively();
	}
}; 

void SceneNode::setScale( const btVector3 & scale )
{		
	this->scale = scale;
};

void SceneNode::setRigidBody( RigidBody * body )
{
	this->body = body;

	this->body->setTransform( globalTransform );
};
