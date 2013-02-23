#pragma once

#include "Scene.h"

class Camera : public SceneNode
{
protected:
	virtual void render( );
	void renderNodeAndChilds( );
	Ray * pickingRay;
public:

	static Camera * activeCamera;
	
	Camera();
	void setFOV( float fovInDeg );
	void setClearColor( Color color );

	SceneNode * pickByRay( )
	{
		return Scene::getInstance()->getRoot()->getNodeByRay( pickingRay );
	}
};

