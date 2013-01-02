#include "Scene.h"

Scene::Scene( )
{
		
};



void Scene::render () 
{
	// render text here
};

void Scene::renderNodeAndChilds( )
{
	// Render childs relatively this node
	for( uint i = 0; i < childs.size(); i++ )
		childs.at( i )->renderNodeAndChilds();
};