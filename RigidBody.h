#pragma once

#include "Surface.h"

class RigidBody
{
protected:
	btRigidBody * body;
public:
	RigidBody()
	{
		body = 0;
	};

	virtual ~RigidBody()
	{
		dynamicsWorld->removeRigidBody( body );

		if( body->getCollisionShape() )
			delete body->getCollisionShape();

		if( body->getMotionState())
			delete body->getMotionState();

		delete body;
	};

	btRigidBody * getPhysBody()
	{
		return body;
	};

	void setTransform( btTransform & transform )
	{
		body->setWorldTransform( transform );
	};

	btTransform & getTransform( )
	{
		return body->getWorldTransform();
	};

	void addForce( const btVector3 & force )
	{
		body->applyCentralForce( force );
	};

	void setLinearVelocity( const btVector3 & linearVel )
	{
		body->setLinearVelocity( linearVel );
	};

	btVector3 getLinearVelocity()
	{
		return body->getLinearVelocity();
	};
};

class SphereBody : public RigidBody
{
public:
	SphereBody( float mass, float radius )
	{
		btCollisionShape * shape = new btSphereShape( radius );
		btVector3 inertia; shape->calculateLocalInertia( mass, inertia );
		body = new btRigidBody( mass, (btMotionState*)( new btDefaultMotionState() ), shape, inertia );
		body->setActivationState( 1 );
		dynamicsWorld->addRigidBody( body );
	};
};

class TriMeshBody : public RigidBody
{
private:
	btTriangleMesh * mesh;

public:
	TriMeshBody( vector< Surface* > * surfaces )
	{
		if( surfaces->size() == 0 )
			return;
			
		btVector3 inertia( 0.0f, 0.0f, 0.0f );

		mesh = new btTriangleMesh( true, false );
	
		for( uint i = 0; i < surfaces->size(); i++ )
		{
			Surface * surface = (*surfaces)[ i ];

			for( uint ic = 0; ic < surface->getIndexCount(); ic+=3 )
			{
				Vertex * a = surface->getVertex( surface->getIndex( ic + 0 ) );
				Vertex * b = surface->getVertex( surface->getIndex( ic + 1 ) );
				Vertex * c = surface->getVertex( surface->getIndex( ic + 2 ) );
				
				mesh->addTriangle( btVector3( a->x, a->y, a->z ), btVector3( b->x, b->y, b->z), btVector3( c->x, c->y, c->z) );
			};
		};		

		body = new btRigidBody( 0, (btMotionState*)( new btDefaultMotionState()), (btCollisionShape*)( new btBvhTriangleMeshShape( mesh, true, true ) ), inertia );

		dynamicsWorld->addRigidBody( body );
	}

	~TriMeshBody()
	{
		delete mesh;
	};
};

class ConvexBody : public RigidBody
{
public:
	ConvexBody( float mass, vector< Surface* > * surfaces )
	{
		if( surfaces->size() == 0 )
			return;
			
		btVector3 inertia( 0.0f, 0.0f, 0.0f );

		btConvexHullShape * convex = new btConvexHullShape( );

		for( uint i = 0; i < surfaces->size(); i++ )
		{
			Surface * surface = surfaces->at( i );

			for( uint vc = 0; vc < surface->getVertexCount(); vc++ )
			{
				Vertex * vertex = surface->getVertex( vc );

				convex->addPoint( btVector3( vertex->x, vertex->y, vertex->z ));
			};
		};		
		
		convex->calculateLocalInertia( mass, inertia );

		body = new btRigidBody( mass, (btMotionState*)( new btDefaultMotionState()), ( btCollisionShape* )( convex ), inertia );

		dynamicsWorld->addRigidBody( body );
	};
};