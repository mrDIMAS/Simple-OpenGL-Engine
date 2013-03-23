#include "buttonNode.h"

ButtonNode::ButtonNode( int w, int h, Font * font, const char * t) : RectNode( 0, 0, w, h, true )
{
	setColor( Color( 90, 0, 0, 255 ));

	frame = new RectNode( 0, 0, w, h, false );
	frame->attachTo( this );

	text = new TextNode( font, 0, 0, t, w );
	text->setPosition( text->getWidth() / 2, ( h - text->getHeight() ) / 2 );
	text->attachTo( frame );
};

bool ButtonNode::isPressed( )
{
	return mouse.lmbHit() && isMouseIn();
};

void ButtonNode::onRender()
{
	RectNode::onRender();

	if( isMouseIn() )
	{	
		frame->setColor( Color( 0, 255, 0, 255 ));
		text->setColor( Color( 0, 255, 0, 255 ));
	}
	else
	{	
		frame->setColor( Color( 200, 0, 0, 255 ));
		text->setColor( Color( 200, 0, 0, 255 ));
	}
};