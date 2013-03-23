#pragma once

#include "RectNode.h"
#include "TextNode.h"

class ButtonNode: public RectNode
{
	RectNode * frame;
	TextNode * text;
public:
	TextNode * getText( )
	{
		return text;
	};
	ButtonNode( int w, int h, Font * font, const char * t );
	bool isPressed( );
	virtual void onRender();
};