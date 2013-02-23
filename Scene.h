#pragma once

#include "SceneNode.h"
#include "Image.h"
#include "Text.h"

class Root : public SceneNode
{
public:
	void render()
	{

	};
};

class Scene
{
private:
	Root * root;

	Scene( )
	{
		root = new Root;
	};

	static Scene * instance;
public:
	
	Root * getRoot()
	{
		return root;
	};

	static Scene * getInstance( ) 
	{		
		if( !instance )
		{
			instance = new Scene();
		};

		return instance;
	};

	void destroy( )
	{
		Text::deleteAll();
		Image::deleteAll();
		Texture::deleteAll();
		delete root;
	};

	void attachNode( SceneNode * node )
	{
		node->attachTo( root );
	};

	void render2D( )
	{
		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		// set ortho projection
		glMatrixMode( GL_PROJECTION );	glPushMatrix(); glLoadIdentity();
		gluOrtho2D( 0, info->current_w, info->current_h, 0 );

		glPushAttrib( GL_LIGHTING );		
		glPushAttrib( GL_CULL_FACE );

		glDisable( GL_LIGHTING );
		glDisable( GL_CULL_FACE );

		// set identity view matrix
		glMatrixMode( GL_MODELVIEW ); glPushMatrix(); glLoadIdentity();

		Image::renderAll();
		Text::renderAll();

		glPopAttrib();
		glPopAttrib();

		glMatrixMode( GL_MODELVIEW );
		glPopMatrix();
		glMatrixMode( GL_PROJECTION );
		glPopMatrix();

		glMatrixMode( GL_MODELVIEW );
	};

	void render3D()
	{
		root->renderNodeAndChilds();
	};
};