#include "ImageNode.h"

void ImageNode::onRender( )
{
	tex->bind();
		
	glBegin( GL_QUADS );		
		
		glTexCoord2f( 0, 1 );
		glVertex3f( x , y, 0 );

		glTexCoord2f( 1, 1 );
		glVertex3f( x + w * scalerx, y , 0 );

		glTexCoord2f( 1, 0 );
		glVertex3f( x + w * scalerx, y + h * scalery, 0 );

		glTexCoord2f( 0, 0 );
		glVertex3f( x , y + h * scalery, 0 );

	glEnd();	

	glBindTexture( GL_TEXTURE_2D, 0 );
};

ImageNode::ImageNode( Texture * data ) : clr( 255, 255, 255, 255 )
{
	tex = data;
	w = data->width;
	h = data->height;
};

void ImageNode::setColor( Color newColor )
{
	clr = newColor;
};