#pragma once

#include "SceneNode.h"

class Camera : public SceneNode
{
protected:
	virtual void render( )
	{
		btVector3 eye  = globalTransform.getOrigin();
		btVector3 look = eye + globalTransform.getBasis().getColumn( 2 );
		btVector3 up   = globalTransform.getBasis().getColumn( 1 );

		gluLookAt( eye.x(),  eye.y(),  eye.z(), 
			      look.x(), look.y(), look.z(), 
				    up.x(),   up.y(),   up.z() );
	};
	
	void renderNodeAndChilds( )
	{
		glMatrixMode( GL_MODELVIEW );
				
		glLoadIdentity();
		
		render(); // apply view matrix
				
		for( uint i = 0; i < childs.size(); i++ )
			childs.at( i )->renderNodeAndChilds();			
	};	

public:

	static Camera * activeCamera;
	
	Camera()
	{
		glMatrixMode( GL_PROJECTION );

		glLoadIdentity();

		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		gluPerspective( 55, (float)info->current_w / (float)info->current_h, 0.1, 1024 );		

		activeCamera = this;
	};

	void setFOV( float fovInDeg )
	{
		glMatrixMode( GL_PROJECTION );

		glLoadIdentity();

		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		gluPerspective( fovInDeg, (float)info->current_w / (float)info->current_h, 0.1, 1024 );	
	};

	void setClearColor( Color color )
	{
		glClearColor( color.getRed(), color.getGreen(), color.getBlue(), 1 );
	};
};

Camera * Camera::activeCamera = 0;