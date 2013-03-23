#include "RectNode.h"

RectNode::RectNode( int x, int y, int w, int h, bool solid )
{
	this->x = x;
	this->y = y;
	this->w = w;
	this->h = h;
	this->solid = solid;
};

void RectNode::onRender() 
{	
	glDisable( GL_TEXTURE_2D );
	
	glBegin( solid ? GL_QUADS : GL_LINE_LOOP );

	glVertex3f( x, y, z );
	glVertex3f( x + w * scalerx , y , z);
	glVertex3f( x + w * scalerx,  y + h * scalery, z );
	glVertex3f( x, y + h * scalery, z );

	glEnd();
	
	glEnable( GL_TEXTURE_2D );
};

bool RectNode::isSolid( )
{
	return solid;
};

void RectNode::setSolid( bool solid )
{
	this->solid = solid;
}