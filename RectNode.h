#pragma once

#include "GUINode.h"

class RectNode : public GUINode
{
private:
	bool solid;
public:
	RectNode( int x, int y, int w, int h, bool solid = false );
	virtual void onRender();
	bool isSolid( );
	void setSolid( bool solid );
};