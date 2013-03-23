#pragma once

#include "GUINode.h"

class TextNode : public GUINode
{
private:
	Font * font;
	string text;
	float kerning;
	int rectW, rectH;
public:
	TextNode( Font * font, int x = 0, int y = 0, const char * text = "", int rw = 0, int rh = 0 );
	void setKerning( float k );
	void setText( const string & txt );
	void setTextEx( const char * txt, ... );
	virtual void onRender( );
};