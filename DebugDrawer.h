#pragma once

#include "Common.h"

class MyDebugDrawer : public btIDebugDraw
{
public:
	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
	{
		glPushAttrib( GL_LIGHTING );

		glDisable( GL_LIGHTING );

		glBegin( GL_LINES );
		glColor4f( color.x(), color.y(), color.z(), 1 );
		glVertex3f( from.x(), from.y(), from.z() );
		glColor4f( color.x(), color.y(), color.z(), 1 );
		glVertex3f( to.x(), to.y(), to.z() );
		glEnd();

		glPopAttrib();
	};

	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{

	};

	void reportErrorWarning(const char* warningString)
	{

	};

	void draw3dText(const btVector3& location,const char* textString)
	{

	}
	
	void setDebugMode(int debugMode)
	{

	};
	
	int	getDebugMode() const 
	{
		return 1;
	};
};