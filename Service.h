#pragma once

#include "Common.h"


void drawRightHandedGizmoXYZWhereZLookToward()
{
/*			| +y
			|
			|
			|____________ +x
		   /
          /
		 / +z
*/
	glMatrixMode( GL_MODELVIEW );

	glDisable( GL_LIGHTING );

	glBegin( GL_LINES );

		// x
		glColor4f( 1, 0, 0, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 100, 0, 0 );
		
		// y
		glColor4f( 0, 1, 0, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 100, 0 );

		// z
		glColor4f( 0, 0, 1, 1 );
		glVertex3f( 0, 0, 0 );
		glVertex3f( 0, 0, 100 );

	glEnd();

	glEnable( GL_LIGHTING );
};

void ethalonViewTransform()
{
	glMatrixMode( GL_MODELVIEW );

	glLoadIdentity();
		
	gluLookAt( 0, 100, 100, 0, 0, 0, 0, 1, 0 );

	glScalef( 1, 1, -1 );
};