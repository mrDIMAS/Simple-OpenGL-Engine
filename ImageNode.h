#pragma once

#include "GUINode.h"

class ImageNode : public GUINode
{
protected:
	Texture * tex;
	Color clr;
	virtual void onRender( );
public:
	ImageNode( Texture * data );
	void setColor( Color newColor );
};