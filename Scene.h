#pragma once

#include "Mesh.h"



class Scene : public SceneNode // :) feeling like a ROOT! :)
{
public:
	Scene( );
	void render () ;
	void renderNodeAndChilds( );
};