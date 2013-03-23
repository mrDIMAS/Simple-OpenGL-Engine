#pragma once

#include "Common.h"
#include "Mouse.h"
#include "Font.h"
#include "Texture.h"

class GUINode // provides common hierarchy for two-dimensional GUI
{
private:
	void applyTransform( );
protected:
	int lx, ly;
	int w, h;
	int x, y;
	float z;
	float lz;
	float lscalerx;
	float lscalery;
	float scalerx;
	float scalery;
	Color color;
	bool visible;
	vector< GUINode* > childs;	
	GUINode * parent;
	void * userPointer;	
	GUINode( bool au );
	virtual void onRender( )
	{
	}
	bool freeMemoryOnUserpointer;	
public:
	static GUINode * gRoot;
	GUINode( );
	virtual ~GUINode();
	void render( );
	void hideAllChilds();
	void showAllChilds();
	float getScalerX();
	float getScalerY();
	void setScalerX( float sclx );
	void setScalerY( float scly );
	void setColor( const Color & clr );
	Color getColor( );
	uint getCountChildren( );
	void setScaler( float sclx, float scly );
	GUINode * getChild( uint i );
	void setUserPointer( void * up, bool freeMemoryOnUserpointer = false );
	void * getUserPointer( );
	void show( );
	void hide( );
	void attachTo( GUINode * parent );
	void setDepth( float depth );
	float getDepth( );
	void detach( );
	void setPosition( int x, int y );
	void setSize( int w, int h );
	virtual int getWidth( );
	virtual int getHeight( );
	int getX();
	int getY();
	int getGlobalX();
	int getGlobalY();
	bool isVisible( );
	bool isTrulyVisible( );
	virtual bool isMouseIn( );
};





