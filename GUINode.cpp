#include "GUINode.h"

GUINode * GUINode::gRoot = 0;

void GUINode::applyTransform( )
{
	if( parent )
	{
		x = parent->x + lx;
		y = parent->y + ly;
		z = parent->z + lz;
		scalerx = parent->scalerx * lscalerx;
		scalery = parent->scalery * lscalery;
	}
	else
	{
		x = lx;
		y = ly;
		z = lz;
		scalerx = lscalerx;
		scalery = lscalery;
	};
};

GUINode::GUINode( bool au ) : color( 255, 255, 255, 255 )
{
	x = 0;
	y = 0;
	z = 0;

	w = 0;
	h = 0;
		
	lx = x;
	ly = y;
	lz = z;		

	scalerx = 1;
	scalery = 1;
	lscalerx = scalerx;
	lscalery = scalery;

	parent = 0;

	visible = true;

	userPointer = 0;

	freeMemoryOnUserpointer = 0;
};

GUINode::GUINode( ) : color( 255, 255, 255, 255 )
{
	if( !gRoot )
	{
		gRoot = new GUINode( true );
	};

	x = 0;
	y = 0;
	z = 0;

	w = 0;
	h = 0;
		
	lx = x;
	ly = y;
	lz = z;		

	scalerx = 1;
	scalery = 1;
	lscalerx = scalerx;
	lscalery = scalery;

	parent = 0;

	visible = true;

	userPointer = 0;

	freeMemoryOnUserpointer = 0;

	attachTo( gRoot );
};

GUINode::~GUINode()
{
	for( uint i = 0; i < childs.size(); i++ )
	{
		if( freeMemoryOnUserpointer )
			delete childs.at( i )->getUserPointer();

		delete childs.at( i );
	};
};

void GUINode::render( )
{
	if( visible )
	{
		applyTransform();
			
		float lastColor[4];
		glGetFloatv( GL_CURRENT_COLOR, lastColor );

		glColor4fv( color.getElements() );

		glMatrixMode( GL_MODELVIEW );  
		glLoadIdentity( );
		glTranslatef( 0, 0, z );		

		onRender();

		for( uint i = 0; i < childs.size(); i++ )
		{
			childs.at( i )->render();
		};

		glColor4fv( lastColor );

		glScalef( 1, 1, 1 );
	};
};

void GUINode::hideAllChilds()
{
	for( uint i = 0; i < childs.size(); i++ )
	{
		childs.at( i )->hide();

		childs.at( i )->hideAllChilds( );
	};
};

void GUINode::showAllChilds()
{
	for( uint i = 0; i < childs.size(); i++ )
	{
		childs.at( i )->show();

		childs.at( i )->showAllChilds( );
	};
};

float GUINode::getScalerX()
{
	return lscalerx;
};

float GUINode::getScalerY()
{
	return lscalery;
};

void GUINode::setScalerX( float sclx ) 
{
	lscalerx = sclx;
};

void GUINode::setScalerY( float scly ) 
{
	lscalery = scly;
};
void GUINode::setColor( const Color & clr )
{
	color = clr;
};

Color GUINode::getColor( )
{
	return color;
};

uint GUINode::getCountChildren( )
{
	return childs.size();
};

void GUINode::setScaler( float sclx, float scly ) 
{
	lscalerx = sclx;
	lscalery = scly;
};

GUINode * GUINode::getChild( uint i )
{
	return childs.at( i );
};

void GUINode::setUserPointer( void * up, bool freeMemoryOnUserpointer )
{
	userPointer = up;

	this->freeMemoryOnUserpointer = freeMemoryOnUserpointer;
};

void * GUINode::getUserPointer( )
{
	return userPointer;
};

void GUINode::show( )
{
	visible = true;
};

void GUINode::hide( )
{
	visible = false;
};

void GUINode::attachTo( GUINode * parent )
{
	detach();

	this->parent = parent;

	parent->childs.push_back( this );
};

void GUINode::setDepth( float depth )
{
	lz = -depth; // because opengl uses z which looks toward. but engine uses z looks forward
};

float GUINode::getDepth( )
{
	return -lz;
};

void GUINode::detach( )
{
	if( parent )
	{
		for( uint i = 0; i < parent->childs.size(); i++ )
		{
			if( parent->childs.at( i ) == this )
			{
				parent->childs.erase( parent->childs.begin() + i );

				break;
			};
		};

		parent = 0;
	};
};

void GUINode::setPosition( int x, int y )
{
	this->lx = x;
	this->ly = y;
};

void GUINode::setSize( int w, int h )
{
	this->w = w; this->h = h;
};

int GUINode::getWidth( )
{
	return w;
}

int GUINode::getHeight( )
{
	return h;
};

int GUINode::getX()
{
	return lx;
};

int GUINode::getY()
{
	return ly;
};

int GUINode::getGlobalX()
{
	return x;
};

int GUINode::getGlobalY()
{
	return y;
};

bool GUINode::isVisible( )
{
	return visible;
};

bool GUINode::isTrulyVisible( )
{
	bool ret = visible;

	if( parent )
	{
		ret *= parent->isVisible();

		if( ret )
			ret *= parent->isTrulyVisible();
	} 

	return ret;
};

bool GUINode::isMouseIn( )
{
	if( !isTrulyVisible() )
		return false;

	int mx = mouse.getX();
	int my = mouse.getY();

	return mx > x && mx < ( x + w * scalerx ) && my > y && my < ( y + h * scalery );
};	