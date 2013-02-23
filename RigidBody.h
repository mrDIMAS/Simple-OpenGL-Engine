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

	void setMass( float mass )
	{
		btVector3 inertia;

		body->getCollisionShape()->calculateLocalInertia( mass, inertia );

		body->setMassProps( mass, inertia );
	};

	bool onGround()
	{
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

		for (int i=0;i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold = dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA				  = static_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obB				  = static_cast<btCollisionObject*>(contactManifold->getBody1());
			btCollisionObject *thisObj			  = static_cast<btCollisionObject*>( body );

			if( obA == thisObj || obB == thisObj )
			{
				if( contactManifold->getNumContacts() > 0 )
				{
					int numContacts = contactManifold->getNumContacts();

					for (int j=0;j<numContacts;j++)
					{
						btManifoldPoint& pt = contactManifold->getContactPoint(j);
						if (pt.getDistance()<0.f)
						{
							const btVector3& ptA = pt.getPositionWorldOnA();
							const btVector3& ptB = pt.getPositionWorldOnB();
							const btVector3& normalOnB = pt.m_normalWorldOnB;

							if( normalOnB.y() > 0.5 )
								return true;
						}
					}
					
				};
			};
		}

		return false;
	};	

	bool hasCollision( )
	{
		int numManifolds = dynamicsWorld->getDispatcher()->getNumManifolds();

		for (int i=0;i < numManifolds; i++)
		{
			btPersistentManifold* contactManifold =  dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
			btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
			btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
			btCollisionObject *thisObj = static_cast<btCollisionObject*>( body );

			if( obA == thisObj || obB == thisObj )
			{
				if( contactManifold->getNumContacts() > 0 )
					return true;
			};
		}

		return false;
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
		body->setUserPointer( this );
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
				Point3 * a = surface->getVertex( surface->getIndex( ic + 0 ) );
				Point3 * b = surface->getVertex( surface->getIndex( ic + 1 ) );
				Point3 * c = surface->getVertex( surface->getIndex( ic + 2 ) );
				
				mesh->addTriangle( btVector3( a->x, a->y, a->z ), btVector3( b->x, b->y, b->z), btVector3( c->x, c->y, c->z) );
			};
		};		

		body = new btRigidBody( 0, (btMotionState*)( new btDefaultMotionState()), (btCollisionShape*)( new btBvhTriangleMeshShape( mesh, true, true ) ), inertia );
		body->setUserPointer( this );
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
				Point3 * vertex = surface->getVertex( vc );

				convex->addPoint( btVector3( vertex->x, vertex->y, vertex->z ));
			};
		};		
		
		convex->calculateLocalInertia( mass, inertia );

		body = new btRigidBody( mass, (btMotionState*)( new btDefaultMotionState()), ( btCollisionShape* )( convex ), inertia );
		body->setUserPointer( this );
		body->activate( true );
		dynamicsWorld->addRigidBody( body );
	};
};