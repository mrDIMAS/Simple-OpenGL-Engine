/*

	OGLShooter

	by mr.dimas 2013

*/

#include "Engine.h"

class Item 
{
	Mesh * mesh;
	float content;
	int contentType;
	string desc;
	string capt;
	void computeDesc()
	{
		desc.clear();

		char buffer[ 128 ] = { 0 };

		switch( contentType )
		{
		case CT_HEALTH:
			sprintf( buffer, "First aid kit. Restores %d health points", (int)content );
			capt = "First aid kit";
			break;
		case CT_HAMMER:
			sprintf( buffer, "Hammer. Durability is %d", (int)content );
			capt = "Hammer";
			break;
		default:
			sprintf( buffer, "Unknown" );
		};

		desc = buffer;
	};
public:
	enum
	{
		CT_HEALTH,
		CT_HAMMER,
	};

	Item( Mesh * mesh )
	{
		setMesh( mesh );
		contentType = CT_HEALTH;
	};

	void setContentType( int type )
	{
		contentType = type;

		computeDesc();
	};

	void setContentCount( float cont )
	{
		content = cont;

		computeDesc();
	};

	void setMesh( Mesh * mesh )
	{
		this->mesh = mesh;
	};

	Mesh * getMesh( )
	{
		return mesh;
	};

	string & getDesc( )
	{
		return desc;
	};

	string & getCapt()
	{
		return capt;
	};
};

// map parser 
class ProLevel
{	
public:
	Mesh * root;

	ProLevel( const char * file )
	{
		root = new Mesh;

		root->loadEO( file );		

		root->setRigidBody( new TriMeshBody( root->getSurfaces()));

		for( uint i = 0; i < root->getChildCount(); i++ )
		{
			Mesh * mesh = (Mesh*)root->getChild( i );

			if( mesh )
			{
				map<string,string> parameters;

				string name = mesh->getName();

				char * delim = "= ;";
				char * pname = 0;
				char * pvar  = 0;
				bool   begin = false;				

				while( true )
				{
					if( !begin )
					{
						pname = strtok( const_cast<char*>( name.c_str() ), delim );
						pvar  = strtok( 0, delim );

						if( pname && pvar )
							parameters[ pname ] = pvar;
						else
							break;

						begin = true;
					}
					else
					{
						pname = strtok( 0, delim );
						pvar  = strtok( 0, delim );

						if( pname && pvar )
							parameters[ pname ] = pvar;
						else
							break;
					};
				};

				for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
				{
					string pname = parm->first;
					string pvar  = parm->second;

					if( pname == "type" )
					{
						if( pvar == "light" )
						{
							createLight( parameters )->attachTo( mesh );
						};
						if( pvar == "mesh" )
						{
							configureMesh( mesh, parameters );
						};
						if( pvar == "item" )
						{
							createItem( mesh, parameters );
						};
					};

					if( pname == "name" )
					{
						mesh->setName( pvar );
					};

					if( pname == "twosided" )
					{
						int parm = atoi( pvar.c_str() );

						if( parm )
							mesh->setTwosidedRender();
					};

					if( pname == "pickbody" )
					{
						if( pvar == "sphere" )
						{
							mesh->setPickObject( new PickSphere( mesh->getBoundingSphereRadius() ));
						};

						if( pvar == "aabb" )
						{
							btVector3 min, max;
							mesh->getBoundingAABBMetrics( min, max );
							mesh->setPickObject( new PickAABB( min, max ));
						};
					};
				};
			}
		};

		Scene::getInstance()->attachNode( root );
	}

	Light * createLight( map<string,string> & parameters )
	{
		float range = 24;

		PointLight * lit = new PointLight;
		lit->setDiffuseColor( Color( 255, 255, 255, 255 ));
		lit->setAmbientColor( Color( 0, 0, 0, 255 ));
		lit->setRange( range );

		for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
		{
			string pname = parm->first;
			string pvar  = parm->second;

			if( pname == "range" )
			{
				lit->setRange( atof( pvar.c_str()));
			};
		};

		return lit;
	};

	Item * createItem( Mesh * mesh, map<string,string> & parameters )
	{
		Item * item = new Item( mesh );

		mesh->setUserPointer( item );

		for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
		{
			string pname = parm->first;
			string pvar  = parm->second;

			if( pname == "contenttype" )
			{
				int type = 0;

				if( pvar == "health" )
					type = Item::CT_HEALTH;
				if( pvar == "hammer" )
					type = Item::CT_HAMMER;

				item->setContentType( type );

				int gggtg=0;
			};

			if( pname == "contentcount" )
			{
				item->setContentCount( atof( pvar.c_str()));
			};
		};

		return item;
	};

	void configureMesh( Mesh * mesh, map<string,string> & parameters )
	{
		for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
		{
			string pname = parm->first;
			string pvar  = parm->second;

			if( pname == "body" )
			{
				if( pvar == "convex" )
				{
					mesh->setRigidBody( new ConvexBody( 1, mesh->getSurfaces()) );
				};
				if( pvar == "static" )
				{
					mesh->setRigidBody( new TriMeshBody( mesh->getSurfaces()) );
				};
			};
		};

		if( mesh->getRigidBody() )
		{
			for( auto parm = parameters.begin(); parm != parameters.end(); parm++ )
			{
				string pname = parm->first;
				string pvar  = parm->second;

				if( pname == "mass" )
				{
					mesh->getRigidBody()->setMass( atof( pvar.c_str()));
				};
			};
		};
	};

	~ProLevel( )
	{
	};
};

struct InventoryCell
{
	Item * item;
	Text * text;
	InventoryCell( Item * i, Text * t )
	{
		item = i;
		text = t;
	};
};

class Inventory
{
public:

};

class Player
{
public:
	Pivot  * pivot;
	Camera * camera;
	vector< Sound * > stepSounds;
	Font * arialFont;
	float pathLen;
	vector<InventoryCell*> items;
	Text * itemDesc;
	Text * invCaption;
	bool inventoryVisible;
	void playStepSound()
	{
		if( pathLen > 200 )
		{
			stepSounds.at( rand() % stepSounds.size() )->play( camera );

			pathLen = 0;
		};
	};

	void gui()
	{
		if( inventoryVisible )
		{
			for( uint i = 0; i < items.size(); i++ )
			{
				InventoryCell * item = items.at( i );

				item->text->show();

				invCaption->show();
				invCaption->setPosition( 50, 10 );

				if( item->text->isMouseIn() )
				{
					item->text->setKerning( 0.85 );
				}
				else
				{
					item->text->setKerning( 0.75 );
				};
			};
		}
		else
		{
			for( uint i = 0; i < items.size(); i++ )
			{
				InventoryCell * item = items.at( i );

				item->text->hide();
				invCaption->hide();
			};
		};
	};

	bool isItemInInventory( Item * it )
	{
		return 0;
	};

	void update( )
	{
		if( inventoryVisible )
		{
			gui();
		}
		else
		{
			const float sp = 10;
			static float angle = 0;
			static float headOffset = 0;
			static bool jumped = 0;

			float speed = 0;
			float strafe = 0;

			if( keyboard.keyPressed( SDLK_w ) )
				speed = sp;
			if( keyboard.keyPressed( SDLK_s ) )
				speed = -sp;
			if( keyboard.keyPressed( SDLK_a ) )
				strafe = -sp;
			if( keyboard.keyPressed( SDLK_d ) )
				strafe =  sp;

			if( ( abs( speed ) > 0 ) && ( abs( strafe ) > 0 ) )
			{
				strafe *= 0.5;
				speed  *= 0.5;		
			}

			if( keyboard.isLShiftPressed() )
			{
				angle += abs( speed ) + abs( strafe );

				strafe *= 2;
				speed  *= 2;
			}
			else
				angle += abs( speed ) + abs( strafe );

			pathLen += abs( strafe ) + abs( speed );

			playStepSound();
		
			// landing
			if( pivot->getRigidBody()->onGround() && jumped  )
			{
				headOffset -= 0.1;

				if( headOffset < -0.8 )
					jumped = false;
			};

			if( !jumped )
			{
				headOffset += -headOffset * 0.05;
			};

			if( keyboard.keyPressed( SDLK_SPACE ) && pivot->getRigidBody()->onGround() )
			{
				pivot->getRigidBody()->setLinearVelocity( btVector3( 0, 10, 0 ));

				jumped = true;
			};
		
			camera->setPosition( btVector3( 0, headOffset + 2.5 + sinf( angle * 0.02 ) * 0.125, 0 ));

			float mys = -mouse.getYSpeed() * 0.2;

			camera->turn( btVector3( mys, 0, 0 ));		
			pivot->turn( btVector3( 0, mouse.getXSpeed() * 0.2, 0 ));
			pivot->move( btVector3( strafe, 0, speed ));
		
			SceneNode * pickedNode = camera->pickByRay();
		
			if( pickedNode )
			{
				Item * item = (Item*)pickedNode->getUserPointer();

				if( item )
				{
					itemDesc->show();
					itemDesc->setText( item->getDesc() );

					if( keyboard.keyPressed( SDLK_e ) )
					{
						items.push_back( new InventoryCell( item, new Text( arialFont, item->getCapt().c_str(), 200, 40 + items.size() * 40 )) );
						item->getMesh( )->attachTo( camera, false );
						item->getMesh( )->setPosition( btVector3( -10, 0, -10 ));
					};
				}
			}
			else
			{
				itemDesc->hide();
			}

			const SDL_VideoInfo * info = SDL_GetVideoInfo();

			mouse.moveTo( info->current_w / 2, info->current_h / 2 );		
		}
		

		if( keyboard.keyPressed( SDLK_TAB ))
		{
			inventoryVisible = !inventoryVisible;
		};
	};
public:
	friend class World;
	
	Player( const btVector3 & pos )
	{
		arialFont = new Font( "Times New Roman", false, false );
		itemDesc = new Text( arialFont, " ", 20, 200 );
		invCaption = new Text( arialFont, "=ITEMS=", 0, 0 );

		pivot = new Pivot();
		
		pivot->setRigidBody( new SphereBody( 1, 3 ));		
		pivot->getRigidBody()->getPhysBody()->setAngularFactor( btVector3( 0, 0, 0 ));
		pivot->getRigidBody()->getPhysBody()->setAnisotropicFriction( btVector3( 1, 0, 1 ));
		pivot->setPosition( pos );

		camera = new Camera();
		camera->setClearColor( Color( 0, 0, 0, 255 ));
		camera->attachTo( pivot ); 
		camera->setPosition( btVector3( 0, 2.5, 0 ));

		pathLen = 0;
		inventoryVisible = 0;

		stepSounds.push_back( new Sound( "step1.ogg" ));
		stepSounds.push_back( new Sound( "step2.ogg" ));
		stepSounds.push_back( new Sound( "step3.ogg" ));
		stepSounds.push_back( new Sound( "step4.ogg" ));
	};

	Camera * getCamera()
	{
		return camera;
	};

	Pivot * getPivot()
	{
		return pivot;
	};
};

class World
{
public:
	ProLevel * curLevel;
	Player   * player;
	SoundCore * sndcore;

	World( )
	{
		curLevel = 0;
		player = 0;
		sndcore = new SoundCore;
	};

	~World( )
	{
		delete curLevel;
		delete player;
		delete sndcore;
	};

	void setLevel( ProLevel * lev )
	{
		curLevel = lev;
	};

	void setPlayer( Player * p )
	{
		player = p;

		player->getPivot()->attachTo( curLevel->root );

		Listener * listener = new Listener;
		listener->attachTo( player->getCamera() );
	};

	void update( )
	{
		player->update();


	};
};

class Button : public Image
{
public:
	static vector< Button* > buttons;

	Button( const char * file, int w, int h ) : Image( Texture::loadTGA( file ) )
	{
		this->setSize( w, h );

		buttons.push_back( this );
	};

	bool isPressed( )
	{
		return mouse.isLeftButtonPressed() && isCursorIn();
	};

	static void updateAll( )
	{
		for( uint i = 0; i < buttons.size(); i++ )
		{
			Button * b = buttons.at( i );

			if( b->isCursorIn() )
			{	
				if( b->getScaler( ) > ( 0.8  ) )
					b->setScaler( b->getScaler( ) * ( 0.98 ) );
			}
			else
			{	
				if( b->getScaler( ) < 1 )
					b->setScaler( b->getScaler( ) * 1.01  );
			}
		};
	};
};

vector< Button* > Button::buttons;

Button * bstart = 0;
Button * boptions = 0;
Button * bexit = 0;
bool inMenu = true;
Image * crosshair = 0;
bool gameStarted = false;
Image * background = 0;
Image * poweredby = 0;
bool logoShown = false;

int defResW = 2560;
int defResH = 1440;

int curResW = defResW;
int curResH = defResH;

float wScaleFactor = 1;
float hScaleFactor = 1;

void compute2DScaleFactor( )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	curResW = info->current_w;
	curResH = info->current_h;

	wScaleFactor = info->current_w / (float) defResW;
	hScaleFactor = info->current_h / (float) defResH;
};

void initMenu( )
{
	int butW = 256 * wScaleFactor;
	int butH = 128 * hScaleFactor;

	int centerX = ( curResW - butW ) / 2;
	int centerY = ( curResH - butH ) / 2;

	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	bstart = new Button( "data\\gui\\button.tga", butW, butH );
	bstart->setPosition( centerX, butH * 2 );

	boptions = new Button( "data\\gui\\buttonexit.tga", butW, butH );
	boptions->setPosition(  centerX, butH * 3.2 );

	bexit = new Button( "data\\gui\\buttonexit.tga", butW, butH );
	bexit->setPosition(  centerX, butH * 4.2 );
};

void hideMenu()
{
	bstart->hide();
	bexit->hide();
	boptions->hide();
	crosshair->show();
	background->hide();
};

void showMenu()
{
	bstart->show();
	bexit->show();
	boptions->show();
	crosshair->hide();
	background->show();
};

void updateMenu()
{

};

World * world = 0;
Player * player = 0;


int main(int argc, char *argv[])
{	
	Renderer * renderer = new Renderer( 1 );
	
	compute2DScaleFactor();

	const SDL_VideoInfo * info = SDL_GetVideoInfo();	

	poweredby = new Image( Texture::loadTGA( "data\\gui\\poweredby.tga" ));
	poweredby->setPosition( info->current_w / 2 - 256, info->current_h / 2 - 256 );	

	background = new Image( Texture::loadTGA( "data\\gui\\background.tga" ) );
	background->setSize( info->current_w, info->current_h );

	initMenu();

	crosshair = new Image( Texture::loadTGA( "data\\gui\\crosshair.tga" ));
	crosshair->setPosition( info->current_w / 2 - 16, info->current_h / 2 - 16 );
	crosshair->hide();

	long lastTime = clock();

	hideMenu();
	crosshair->hide();

	Pivot * testPoint = new Pivot();
	Scene::getInstance()->attachNode( testPoint );

	while( true )
	{
		renderer->update();

		renderer->beginRender();

		if( keyboard.keyPressed( SDLK_ESCAPE ))
		{
			inMenu = 1;

			showMenu();
		};	

		if( inMenu )
		{
			if( logoShown )
			{
				updateMenu();				

				if( bstart->isPressed())
				{
					inMenu = 0;

					hideMenu();

					clock_t t = clock();

					if( !gameStarted )
					{						
						world = new World;
						player = new Player( btVector3( 0, 5, 0 ));
						world->setLevel( new ProLevel( "data\\levels\\arrival\\arrival.eo" ));
						world->setPlayer( player  );

						PointLight * flashlight = new PointLight;
						flashlight->setDiffuseColor( Color( 255, 255, 255, 255 ));
						flashlight->setRange( 20 );
						flashlight->attachTo( player->getCamera() );	

						gameStarted = true;
					};
				};

				if( bexit->isPressed())
				{
					break;
				};
			}
			else
			{
				//if( ( clock() - lastTime ) > CLOCKS_PER_SEC * 6 )
				//{
					logoShown = true;

					poweredby->hide();

					showMenu();
				//};
			};

			Button::updateAll();
		}
		else
		{			
			world->update();
			crosshair->show();
			SoundChannel::updateAll();
			Scene::getInstance()->render3D();
		};

		mouse.update();

		Scene::getInstance()->render2D();

		renderer->endRender( true, false );
	};
	
	Scene::getInstance()->destroy();

	delete world;

	delete renderer;
	
	return 0;
}