/*

	OGLShooter

	by mr.dimas 2013

*/

#include "Engine.h"
#include "GUINode.h"

int defResW = 800;
int defResH = 600;

int curResW = defResW;
int curResH = defResH;

float wScaleFactor = 1;
float hScaleFactor = 1;

World * world = 0;
Player * player = 0;

Dynamic::Parser::ValueMap generalParameters;

// map parser 
class ProLevel
{	
public:
	Mesh * root;

	ProLevel( const char * file )
	{
		root = new Mesh;

		root->loadEO( file );		

		Scene::getInstance()->attachNode( root );
	}

	~ProLevel( )
	{

	};
};

Font * font = 0;

class Inventory : public ImageNode
{
	SceneNode * selectedItem;
	TextNode * itemDesc;
	TextNode * selItemDesc;
	ButtonNode * useItemButton;
	vector<TextNode*> items;
	SceneNode * itemForUse;
public:
	Inventory( Texture * tex ) : ImageNode( tex )
	{
		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		itemForUse = 0;

		selectedItem = 0;

		int defW = 600, defH = 400;
		int scrLeftOffset = ( info->current_w - defW ) / 2;
		int scrTopOffset  = ( info->current_h - defH ) / 2;

		setSize( defW, defH );
		setPosition( scrLeftOffset, scrTopOffset );

		TextNode * caption = new TextNode( font, 40, 0, "ПРЕДМЕТЫ" );
		caption->setPosition( ( getWidth() - caption->getWidth( ) ) / 2, 20 );
		caption->attachTo( this );

		itemDesc = new TextNode( font, 20, defH - 50, "", defW - 20 );
		itemDesc->setScaler( 0.5, 0.5 );
		itemDesc->attachTo( this );

		selItemDesc = new TextNode( font, defW - 200, 70, "", defW - 20 );
		selItemDesc->setScaler( 0.5, 0.5 );
		selItemDesc->attachTo( this );		

		useItemButton = new ButtonNode( 40, 40, font, "Исп." );
		useItemButton->getText( )->setScaler( 0.5, 0.5 );
		useItemButton->setPosition( defW - 80, defH - 80 );
		useItemButton->attachTo( this );
	};

	~Inventory( )
	{

	};

	void addItem( SceneNode * item )
	{
		TextNode * newItem = new TextNode( font, 0, 0, item->getProperty( "desc" ) );
		newItem->setScaler( 0.5, 0.5 );
		newItem->setColor( Color( 0, 100, 0, 255 ));
		newItem->setDepth( -0.0001 );
		newItem->attachTo( this );
		newItem->setUserPointer( item );
		items.push_back( newItem );

		RectNode * frame = new RectNode( -40, -5, newItem->getWidth(), newItem->getHeight() + 5, true );
		frame->setColor( Color( 100, 0, 0, 255 ));
		frame->setDepth( 0.0001 );
		frame->attachTo( newItem );		

		resortItems();
	};

	void resortItems( )
	{
		for( uint i = 0; i < items.size(); i++ )
		{
			items.at( i )->setPosition( 40, 70 + i * 26 );
		};
	};

	SceneNode * getSelectedItem( )
	{
		return selectedItem;
	};

	SceneNode * getItemForUse( )
	{
		SceneNode * it = itemForUse;

		itemForUse = 0;

		return it;
	};

	void deleteItem( SceneNode * itemToDel )
	{
		for( uint i = 0; i < items.size(); i++ ) // update interaction with items
		{
			TextNode * text = items.at( i );

			if( text )
			{
				SceneNode * item = (SceneNode*)text->getUserPointer();

				if( item  )
				{
					if( item == itemToDel )
					{
						text->detach();								

						delete text;

						items.erase( items.begin() + i );

						resortItems();
					}			
				}
			}
		}
	};

	virtual void onRender( )
	{
		ImageNode::onRender();

		itemDesc->hide();

		for( uint i = 0; i < items.size(); i++ ) // update interaction with items
		{
			TextNode * text = items.at( i );

			if( text )
			{
				SceneNode * item = (SceneNode*)text->getUserPointer();

				if( item )
				{
					if( text->isMouseIn() )
					{
						text->setKerning( 0.85 );
						text->getChild( 0 )->setSize( text->getWidth(), text->getHeight() + 5 );
						text->setColor( Color( 0, 220, 0, 255 ));
						itemDesc->setText( item->getProperty( "caption" ));
						itemDesc->show();

						if( mouse.lmbHit( ))
						{
							selectedItem = item;

							selItemDesc->setText( string( "Предмет в руках:\n" ).append( item->getProperty( "desc" ).getStr()));
						};
					}
					else
					{
						text->setKerning( 0.7 );
						text->getChild( 0 )->setSize( text->getWidth(), text->getHeight() + 5 );
						text->setColor( Color( 0, 100, 0, 255 ));						
					};				
				}
			}
		}

		if( selectedItem )
		{
			useItemButton->show();

			if( useItemButton->isPressed())
			{
				if( selectedItem->getProperty( "item" ).getStr() == "heal" )
				{
					itemForUse = selectedItem;

					deleteItem( selectedItem );
				};
			};
		}
		else
			useItemButton->hide();
	};
};

class HUD : public ImageNode
{
public:
	TextNode * healthNode;
	TextNode * pickedItemDesc;

	HUD( Texture * back, float * health, SceneNode ** itemForDesc ) : ImageNode( back )
	{
		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		int defW = 600, defH = 100;
		int scrLeftOffset = ( info->current_w - defW ) / 2;
		int scrTopOffset  = ( info->current_h - defH ) ;

		setSize( defW, defH );
		setPosition( scrLeftOffset, scrTopOffset );

		healthNode = new TextNode( font, defW - 330, 30 );
		healthNode->setUserPointer( health );
		healthNode->attachTo( this );
		healthNode->setScaler( 0.5, 0.5 );

		pickedItemDesc = new TextNode( font, 20, 30 );
		pickedItemDesc->setUserPointer( itemForDesc );
		pickedItemDesc->attachTo( this );
		pickedItemDesc->setScaler( 0.5, 0.5 );
	};

	virtual void onRender()
	{
		ImageNode::onRender();

		float health = *((float*)healthNode->getUserPointer());

		if( health >= 90 )
			healthNode->setText( "Чувствую себя отлично" );
		if( health >= 70 && health < 90 )
			healthNode->setText( "Чувствую себя хорошо.\nМаленькие порезы и царапины" );
		if( health >= 50 && health < 70 )
			healthNode->setText( "Чувствую себя нормально.\nНесколько глубоких порезов.\nДумаю кровь сама остановится" );
		if( health >= 30 && health < 50 )
			healthNode->setText( "Чувствую себя плохо.\nМного глубоких порезов.\nЯ истекаю кровью.\nНужно перевязать раны." );
		if( health >= 5 && health < 30 )
			healthNode->setText( "Я присмерти.\nНужно вколоть морфий и перевязать раны." );

		SceneNode * item = (SceneNode*)( *((SceneNode**)pickedItemDesc->getUserPointer() ));

		if( item )
		{
			pickedItemDesc->setText( item->getProperty( "desc" ) );
			pickedItemDesc->show();
		}
		else
		{
			pickedItemDesc->hide();
		};
	};
};

class Notes : public ImageNode
{
	int visNote;
	vector< TextNode* > notes;
	TextNode * caption;
	ButtonNode * nextButton;
	ButtonNode * prevButton;
public:
	Notes( Texture * back ) : ImageNode( back )
	{
		const SDL_VideoInfo * info = SDL_GetVideoInfo();

		visNote = 0;

		int defW = 600, defH = 400;
		int scrLeftOffset = ( info->current_w - defW ) / 2;
		int scrTopOffset  = ( info->current_h - defH ) / 2;

		setSize( defW, defH );
		setPosition( scrLeftOffset, scrTopOffset );

		caption = new TextNode( font, 40, 0, "ЗАМЕТКИ" );
		caption->setPosition( ( defW - caption->getWidth( ) ) / 2, 20 );
		caption->attachTo( this );

		nextButton = new ButtonNode( 32, 32, font, ">" );
		nextButton->setPosition( defW - 35, defH - 35 );
		nextButton->attachTo( this );

		prevButton = new ButtonNode( 32, 32, font, "<" );
		prevButton->setPosition( 3, defH - 35 );
		prevButton->attachTo( this );

		addNote( "Прибытие.\n"
				 "Наконец-то я нашел это место! Сейчас 3.10 ночи 26 июня 2000 года. Я выехал в 12.00 24 июня. Дорога была не из легких, но полученный два дня назад аванс и озвученный остаток суммы сильно подогревали интерес к этому делу.\n"
				 "Пару слов обо мне:\n"
				 "Я - частный детектив, специализирующийся на особо сложных делах\n"
				 "Теперь о деле:\n"
				 "Мне поручено найти убийцу дочери моего клиента и привести этого изверга живьем. Последний раз этого человека видели именно в этом здании.\n"
				 "Мне нужно попасть внутрь, возможно там я смогу найти его."
			     "Это место уже много десятилетий стояло нетронутым, и кто знает что в нем проиходило в течении этого времени."
				 "Но сейчас тут стоят новые ящики из под чего-то очень большого. "
				 "Здесь точно кто-то есть! "
				 "Мне следует быть внимательным и острожным."  );

		visNote = 0;
	};

	void addNote( string text )
	{
		TextNode * note = new TextNode( font, 20, 45, text.c_str(), getWidth() - 40 );		
		note->attachTo( this );	
		note->setScaler( 0.5, 0.5 );
		notes.push_back( note );
		visNote = notes.size() - 1;		
		pfPlaySound( pfCreateSound( pfDataLoad( "data/sounds/note.ogg", false ), false, true ), 1 );
		show();
	};

	void hideAllNotes()
	{
		for( uint i = 0; i < notes.size() ; i++ )
		{
			notes.at( i )->hide();
		};
	};

	virtual void onRender( )
	{
		ImageNode::onRender();

		if( prevButton->isPressed() )
			if( visNote >= 1 )
				visNote--;

		if( nextButton->isPressed() )
		{
			if( visNote < notes.size() - 1 )
			{
				visNote++;
			}
		};

		hideAllNotes();
		notes.at( visNote )->show();
	};
};

class Player
{
public:
	Pivot  * pivot;
	Pivot * stepsEmitter;
	Pivot * lookPoint;
	Camera * camera;
	PointLight * flashlight;
	float pathLen;
	float health;
	SceneNode * pickedItem;
	float flashLightCharge;
	HUD * hud;
	Inventory * inventory;
	Notes * notes;
	TextNode * info;
	TextNode * doorInfo;

	int timer;

	~Player( )
	{

	};

	void playStepSound()
	{
		if( pathLen > 200 )
		{
			//stepSounds.at( rand() % stepSounds.size() )->play( camera );
			stepsEmitter->playRandomSoundFromGroup( "step", true );

			pathLen = 0;
		};
	};

	void useItem( SceneNode * item )
	{
		if( item )
		{
			if( item->getProperty( "item" ).getStr() == "heal" )
				health += item->getProperty( "contentcount" ).getNum() ;
		};

	};

	void stop( )
	{
		pivot->getRigidBody()->setLinearVelocity( btVector3( 0, 0, 0 ));
	};

	void update( )
	{
		Scene::getInstance()->beginRender2D();

			hud->render();
			inventory->render();	
			notes->render();
			useItem( inventory->getItemForUse() );
			doorInfo->render();

		Scene::getInstance()->endRender2D();	



		btVector3 look = lookPoint->getGlobalPosition( true )  -  camera->getGlobalPosition( true );
		float flook[ ] = { look.x(), look.y(), look.z() };
		float fup[ ] =  { 0, 1, 0 };

		pfSetListenerOrientation( flook, fup );

		if( !inventory->isVisible() && !notes->isVisible() )
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

			pathLen += abs( strafe ) / 2 + abs( speed ) / 2;

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

			if( health > 30 && health < 50 )
			{
				strafe *= 0.5;
				speed  *= 0.5;
			};
			if( health <= 30 )
			{
				strafe *= 0.25;
				speed  *= 0.25;
			};			

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

			int pitchLimit = 85;
			
			if( camera->getRotation().z() - mys > pitchLimit  || camera->getRotation().z() - mys < -pitchLimit)
				mys = 0;	

			camera->turn( btVector3( mys, 0, 0 ));	

			pivot->turn( btVector3( 0, mouse.getXSpeed() * 0.2, 0 ));
			pivot->move( btVector3( strafe, 0, speed ));
		
			SceneNode * pickedNode = camera->pickByRay();
		
			pickedItem = 0;
			timer = 0;

			doorInfo->hide();

			if( pickedNode )
			{
				if( pickedNode->hasProperty( "item" ) )
				{
					if( pickedNode->getDistanceTo( camera ) < 15 )
					{
						pickedItem = pickedNode;

						if( keyboard.keyPressed( SDLK_e ) )
						{
							if( pickedNode->getProperty( "item" ).getStr() != "note" )
							{
								inventory->addItem( pickedNode );		
							}
							else
							{
								notes->addNote( pickedNode->getProperty( "note" ).getStr());
							};

							pickedNode->setPosition( btVector3( -100000, 0, -100000 ));
						};
					}			

				}

				if( pickedNode->hasProperty( "door" ) )
				{
					if( pickedNode->getProperty( "door" ).getStr() == "locked" )
					{
						doorInfo->setText( "Дверь закрыта. Нужен ключ." );
						SceneNode * item = inventory->getSelectedItem();

						if( item )
						{
							if( mouse.lmbHit( ) )
							{
								if( item->hasProperty( "key" ))
								{
									if( pickedNode->getProperty( "key" ).getStr() == item->getProperty( "key" ).getStr() )
									{
										doorInfo->setText( "Дверь открыта." );

										pickedNode->setProperty( "door", Value( "unlocked" ));
									}	
								}
							};
						}
					}
					doorInfo->show();
				};
			}

			const SDL_VideoInfo * info = SDL_GetVideoInfo();
			mouse.moveTo( info->current_w / 2, info->current_h / 2 );		
		}
		else
		{
			stop();
		};

		if( keyboard.keyHit( SDLK_TAB ))
		{
			notes->hide();

			if( inventory->isVisible() )
			{
				inventory->hide();				
				const SDL_VideoInfo * info = SDL_GetVideoInfo();
				mouse.moveTo( info->current_w / 2, info->current_h / 2 );		
				mouse.flush();
			}
			else
				inventory->show();
		};

		if( keyboard.keyHit( SDLK_j ))
		{			
			inventory->hide();

			if( notes->isVisible() )
			{
				notes->hide();
				const SDL_VideoInfo * info = SDL_GetVideoInfo();
				mouse.moveTo( info->current_w / 2, info->current_h / 2 );		
				mouse.flush();
			}
			else
				notes->show();
		};

		static bool flenabled = false;

		if( flenabled )
		{
			if( flashLightCharge > 0.01 )
			{
				flashLightCharge -= 0.01;

				flashlight->setRange( flashLightCharge );
			}
			else
			{
				flenabled = false;
			};
		}
		else
		{
			if( flashLightCharge < 20 )
				flashLightCharge += 0.075;
		};



		if( keyboard.keyHit( SDLK_f ))
		{
			if( flenabled )
			{
				flashlight->setRange( 0.001 );
				flenabled = 0;
			}
			else
			{
				
				flenabled = 1;
			}
		};
	};
public:
	friend class World;
	
	Player( const btVector3 & pos )
	{
		inventory = new Inventory( Texture::loadTGA( "data\\gui\\background.tga" ));
		inventory->hide();

		hud = new HUD( Texture::loadTGA( "data\\gui\\background.tga" ), &health, &pickedItem );

		notes = new Notes( Texture::loadTGA( "data\\gui\\background.tga" ) );		 
		notes->hide();

		info = new TextNode( font, 300, 100 );
		info->setScaler( 0.6, 0.6 );

		doorInfo = new TextNode( font, 300, 200 );
		doorInfo->setScaler( 0.6, 0.6 );		

		pickedItem = 0;		
		flashLightCharge = 20;
		health = 100;

		pivot = new Pivot();
		
		stepsEmitter = new Pivot();
		stepsEmitter->attachTo( pivot );
		stepsEmitter->setPosition( btVector3( 0, 0, 1.5 ));

		pivot->setRigidBody( new SphereBody( 1, 3 ));		
		pivot->getRigidBody()->getPhysBody()->setAngularFactor( btVector3( 0, 0, 0 ));
		pivot->getRigidBody()->getPhysBody()->setAnisotropicFriction( btVector3( 1, 0, 1 ));
		pivot->setPosition( pos );
		pivot->addProperty( "name", "player" );

		camera = new Camera();
		camera->setClearColor( Color( 0, 0, 0, 255 ));
		camera->attachTo( pivot ); 
		camera->setPosition( btVector3( 0, 2.5, 0 ));

		lookPoint = new Pivot( );

		lookPoint->attachTo( pivot );
		lookPoint->setPosition( btVector3( 0, 0, 1 ));

		pathLen = 0;		

		stepsEmitter->addSoundToGroup( "step", pfCreateSound( pfDataLoad( "data\\sounds\\step1.ogg" ), true ));
		stepsEmitter->addSoundToGroup( "step", pfCreateSound( pfDataLoad( "data\\sounds\\step2.ogg" ), true ));
		stepsEmitter->addSoundToGroup( "step", pfCreateSound( pfDataLoad( "data\\sounds\\step3.ogg" ), true ));
		stepsEmitter->addSoundToGroup( "step", pfCreateSound( pfDataLoad( "data\\sounds\\step4.ogg" ), true ));

		flashlight = new PointLight;
		flashlight->setDiffuseColor( Color( 255, 255, 255, 255 ));
		flashlight->setRange( 0.001 );
		flashlight->attachTo( getCamera() );	
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

	World( )
	{
		curLevel = 0;
		player = 0;
	};

	~World( )
	{
		delete curLevel;
		delete player;
	};

	void setLevel( ProLevel * lev )
	{
		curLevel = lev;
	};

	void setPlayer( Player * p )
	{
		player = p;

		player->getPivot()->attachTo( curLevel->root );
	};

	void update( )
	{
		player->update();
	};
};

bool gameStarted = false;

void compute2DScaleFactor( )
{
	const SDL_VideoInfo * info = SDL_GetVideoInfo();

	curResW = info->current_w;
	curResH = info->current_h;

	wScaleFactor = info->current_w / (float) defResW;
	hScaleFactor = info->current_h / (float) defResH;
};

namespace Script
{
	using namespace Dynamic;

	vector< uint > loopSounds;

	void addForce( Function::ArgumentList & args )
	{
		if( args.size() < 4 ) // node_name x_force y_force z_force
			return;

		SceneNode * node = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		if( node )
		{
			RigidBody * body = node->getRigidBody( );

			if( body )
				body->addForce( btVector3( atof( args.at( 1 ).c_str() ), atof( args.at( 2 ).c_str() ), atof( args.at( 3 ).c_str() ) ));
		}
	};

	void hideObject( Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name 
			return;

		SceneNode * node = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		if( node )
		{
			node->hide();
		}
	};

	void showObject( Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name 
			return;

		SceneNode * node = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		if( node )
		{
			node->show();
		}
	};

	void placeNear( Function::ArgumentList & args )
	{
		if( args.size() < 2 ) // node_name node_name_where_placed
			return;

		SceneNode * src = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );
		SceneNode * dst = Scene::getInstance()->getRoot()->findByNameProp( args.at( 1 ) );

		if( src && dst )
		{
			src->setGlobalPosition( dst->getGlobalPosition( true ));
		}
	};

	void disableAllLights( Function::ArgumentList & args )
	{
		globalRenderFlags &= ~GRF_USELIGHT;
	};

	void enableLights( Function::ArgumentList & args )
	{
		globalRenderFlags |= GRF_USELIGHT;
	};

	void turnoffLight(  Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name
			return;

		SceneNode * src = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		Light * lit = dynamic_cast<Light*>( src->getChild( 0 ) );

		if( lit )
		{
			lit->setRangeMult( 0.0 );
		};
	};

	void turnoffParticleSystem(  Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name
			return;

		SceneNode * src = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		ParticleSystem * ps = dynamic_cast<ParticleSystem*>( src->getChild( 0 ) );

		if( ps )
		{
			ps->hide();
		};
	};

	void turnonParticleSystem(  Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name
			return;

		SceneNode * src = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		ParticleSystem * ps = dynamic_cast<ParticleSystem*>( src->getChild( 0 ) );

		if( ps )
		{
			ps->show();
		};
	};

	void turnonLight(  Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // node_name
			return;

		SceneNode * src = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		Light * lit = dynamic_cast<Light*>( src->getChild( 0 ) );

		if( lit )
		{
			lit->setRangeMult( 1 );
		};
	};

	void enableTrigger( Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // trig_name
			return;

		SceneNode * trig = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		if( trig )
		{
			if( trig->hasProperty( "enabled" ) )
			{
				trig->setProperty( "enabled", Value( "1" ));
			};
		}
	};

	void disableTrigger( Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // trig_name
			return;

		SceneNode * trig = Scene::getInstance()->getRoot()->findByNameProp( args.at( 0 ) );

		if( trig )
		{
			if( trig->hasProperty( "enabled" ) )
			{
				trig->setProperty( "enabled", Value( "0" ));
			};
		}
	};


	void addNote( Function::ArgumentList & args )
	{
		if( args.size() < 1 ) // note
			return;

		player->notes->addNote( args.at( 0 ));
	};

	void playSound( Function::ArgumentList & args )
	{
		if( args.size() < 2 ) // file_name emitter_name 
			return;

		SceneNode * emitter = Scene::getInstance()->getRoot()->findByNameProp( args.at( 1 ) );

		Sound snd = pfCreateSound( pfDataLoad( args.at( 0 ).c_str()), true, true );
		pfPlaySound( snd, true );
		pfSetSoundPosition( snd, emitter->getGlobalPosition( true ).x(), emitter->getGlobalPosition( true ).y(), emitter->getGlobalPosition( true ).z() );
		pfSetSoundVolume( snd, 0.7 ); // special for screamers
	};

	void playSoundLoop( Function::ArgumentList & args )
	{
		if( args.size() < 2 ) // file_name emitter_name [2d] [volume]
			return;

		SceneNode * emitter = Scene::getInstance()->getRoot()->findByNameProp( args.at( 1 ) );

		bool two = false;

		if( args.size() > 1 )
			two = atoi( args.at( 2 ).c_str());

		Sound snd = pfCreateSound( pfDataLoad( args.at( 0 ).c_str()), two, true );
		pfPlaySound( snd, true );

		if( !two )
			pfSetSoundPosition( snd, emitter->getGlobalPosition( true ).x(), emitter->getGlobalPosition( true ).y(), emitter->getGlobalPosition( true ).z() );

		if( args.size() > 2 ) 
			pfSetSoundVolume( snd, atof( args.at( 3 ).c_str() ) ); // special for screamers

		loopSounds.push_back( snd );
	};

	void playSoundEx( Function::ArgumentList & args )
	{
		if( args.size() < 3 ) // file_name emitter_name volume
			return;

		SceneNode * emitter = Scene::getInstance()->getRoot()->findByNameProp( args.at( 1 ) );

		Sound snd = pfCreateSound( pfDataLoad( args.at( 0 ).c_str()), true, true );
		pfPlaySound( snd, true );
		pfSetSoundPosition( snd, emitter->getGlobalPosition( true ).x(), emitter->getGlobalPosition( true ).y(), emitter->getGlobalPosition( true ).z() );
		pfSetSoundVolume( snd, atof( args.at( 2 ).c_str() ) ); // special for screamers
	};

	void play2DSound( Function::ArgumentList & args )
	{
		if( args.size() < 2 ) // file_name volume
			return;
		
		Sound snd = pfCreateSound( pfDataLoad( args.at( 0 ).c_str()), 0, true );
		pfPlaySound( snd, true );
		pfSetSoundVolume( snd, atof( args.at( 1 ).c_str() ) ); // special for screamers
	};

	int registerAllFunctions( )
	{
		Function::Register( "addForce", addForce );
		Function::Register( "placeNear", placeNear );

		Function::Register( "addNote", addNote );

		Function::Register( "enableTrigger", enableTrigger );
		Function::Register( "disableTrigger", disableTrigger );

		Function::Register( "playSound", playSound );
		Function::Register( "playSoundEx", playSoundEx );
		Function::Register( "play2DSound", play2DSound );
		Function::Register( "playSoundLoop", playSoundLoop );	

		Function::Register( "disableAllLights", disableAllLights );
		Function::Register( "enableLights", enableLights );

		Function::Register( "turnonLight", turnonLight );
		Function::Register( "turnoffLight", turnoffLight );

		Function::Register( "turnonParticleSystem", turnonParticleSystem );
		Function::Register( "turnoffParticleSystem", turnoffParticleSystem );

		Function::Register( "hideObject", hideObject );
		Function::Register( "showObject", showObject );

		return 1;
	};

	vector<SceneNode*> triggers;

	void findAllTriggers( )
	{
		for( uint i = 0; i < SceneNode::nodes.size(); i++ )
		{
			SceneNode * node = SceneNode::nodes.at( i );

			if( node->hasProperty( "type" ))
			{
				if( node->getProperty( "type" ).getStr() == "distTrigger" )
				{
					triggers.push_back( node );

					if( !node->hasProperty( "enabled" ))
						node->addProperty( "enabled", "1" );

					if( !node->hasProperty( "noDisable" ))
						node->addProperty( "noDisable", "0" );

					if( !node->hasProperty( "trigDistance" ))
						node->addProperty( "trigDistance", "10" );
				};

				if( node->getProperty( "type" ).getStr() == "permanentTrigger" )
				{
					triggers.push_back( node );
				}

			}
		};
	};
	
	void updateAllTriggers( )
	{
		for( auto it = loopSounds.begin(); it != loopSounds.end(); it++ )
		{
			if( !pfIsSoundPlaying( *it ))
			{
				pfPlaySound( *it, true );
			}
		};

		for( uint i = 0; i < triggers.size(); i++ )
		{
			SceneNode * trig = triggers.at( i );

			bool enabled = trig->getProperty( "enabled" ).getStr() == "1";

			if( enabled )
			{
				if( trig->getProperty( "type" ).getStr() == "distTrigger" )
				{
					float dist = trig->getProperty( "trigDistance" );

					if( player->getPivot()->getDistanceTo( trig ) <= dist )
					{
						if( trig->getProperty( "noDisable" ).getStr() == "0" )
							trig->setProperty( "enabled", Value( "0" ));

						for( auto func = trig->properties.begin(); func != trig->properties.end(); func++ )
						{
							Function::ArgumentList args; string funcName; 

							if( func->first.find( "whenApproaching" ) != string::npos )
							{
								Parser::parseFunction( func->second, funcName, args );
								Function::Call( funcName, args );
							}
						}					
					};
				}

				if( trig->getProperty( "type" ).getStr() == "permanentTrigger" )
				{
					if( trig->getProperty( "noDisable" ).getStr() == "0" )
						trig->setProperty( "enabled", Value( "0" ));

					for( auto func = trig->properties.begin(); func != trig->properties.end(); func++ )
					{
						Function::ArgumentList args; string funcName; 

						if( func->first.find( "action" ) != string::npos )
						{
							Parser::parseFunction( func->second, funcName, args );
							Function::Call( funcName, args );
						}
					}					
				}

			};
		}
	};
};


int main(int argc, char *argv[])
{	
	Dynamic::Parser::parseFile( "default.cfg", generalParameters );

	Renderer * renderer = new Renderer( 
		static_cast<unsigned int>( generalParameters[ "width" ].getNum() ), 
		static_cast<unsigned int>( generalParameters[ "height" ].getNum() ), 
		static_cast<unsigned int>( generalParameters[ "fullscreen" ].getNum() ) );		

	pfSetListenerSetEAXPreset( PRESET_FOREST );

	//Sound music = pfCreateSound( pfDataLoad( "data\\music\\notch.ogg", true ) );

	pfSetListenerDopplerFactor( 0 );
	pfSetListenerRolloffFactor( 0.18 );

	compute2DScaleFactor();

	const SDL_VideoInfo * info = SDL_GetVideoInfo();	

	int butW = 200;
	int butH = 90;

	int centerX = ( curResW - butW ) / 2 - 60;
	int centerY = ( curResH - butH ) / 2;	

	ImageNode * menuRoot = new ImageNode( Texture::loadTGA( "data\\gui\\background.tga" ));
	menuRoot->setSize( info->current_w, info->current_h );

	font = new Font( "Arial", 0, 0 );

	ButtonNode * startButton = new ButtonNode( butW, butH, font, generalParameters[ "startButton" ].getStr().c_str() );
	startButton->setPosition( centerX, butH * 2 );
	startButton->attachTo( menuRoot );

	ButtonNode * exitButton = new ButtonNode( butW, butH, font, generalParameters[ "exitButton" ].getStr().c_str() );
	exitButton->setPosition( centerX, butH * 4.4 );
	exitButton->attachTo( menuRoot );	

	ButtonNode * optionsButton = new ButtonNode( butW, butH, font, generalParameters[ "optionsButton" ].getStr().c_str() );
	optionsButton->setPosition( centerX, butH * 3.2 );
	optionsButton->attachTo( menuRoot );

	ImageNode * optFrame = new ImageNode( Texture::loadTGA( "data\\gui\\background.tga" ) );
	optFrame->setSize( 300, 600 );
	optFrame->setPosition( centerX + butW + 60, 0 );
	optFrame->attachTo( menuRoot );
	optFrame->hide();

	ButtonNode * texHighQuality = new ButtonNode( 100, 50, font, generalParameters[ "optTexHighQuality" ].getStr().c_str() );
	texHighQuality->setPosition( 20, 60 );
	texHighQuality->attachTo( optFrame );

	ButtonNode * texMedQuality = new ButtonNode( 100, 50, font, generalParameters[ "optTexMedQuality" ].getStr().c_str() );
	texMedQuality->setPosition( 20, 120 );
	texMedQuality->attachTo( optFrame );

	ButtonNode * texLowQuality = new ButtonNode( 100, 50, font, generalParameters[ "optTexLowQuality" ].getStr().c_str() );
	texLowQuality->setPosition( 20, 190 );
	texLowQuality->attachTo( optFrame );

	TextNode * text = new TextNode( font, curResW - 200, 0, "Test build" );
	text->setScaler( 0.6, 0.6 );
	
	TextNode * helpNode = new TextNode( font, 20, 20, generalParameters[ "help" ].getStr().c_str() );
	helpNode->setScaler( 0.5, 0.5 );

	Script::registerAllFunctions();

	TextNode * debugText = new TextNode( font, 20, 120, "" );
	debugText->setScaler( 0.5, 0.5 );

	while( true )
	{
		renderer->update();

		renderer->beginRender();
		
		if( keyboard.keyPressed( SDLK_ESCAPE ))
		{
			menuRoot->show();
		};	
				
		if( startButton->isPressed( ) )
		{
			if( !gameStarted )
			{						
				world = new World;
				player = new Player( btVector3( 0, 5, 0 ));
				world->setLevel( new ProLevel( "data\\levels\\arrival\\arrival.eo" ));
				world->setPlayer( player  );

				Script::findAllTriggers();

				gameStarted = true;
			};

			menuRoot->hide();
		};

		if( exitButton->isPressed() )
		{
			break;
		};

		if( texHighQuality->isPressed() )
		{
			Texture::loadQuality = Texture::QUALITY_BEST;
		};

		if( texMedQuality->isPressed() )
		{
			Texture::loadQuality = Texture::QUALITY_MEDIUM;
		};

		if( texLowQuality->isPressed() )
		{
			Texture::loadQuality = Texture::QUALITY_LOW;
		};

		if( optionsButton->isPressed() )
		{
			if( optFrame->isTrulyVisible() )
				optFrame->hide();
			else
				optFrame->show();
		};

		if( !menuRoot->isVisible() )
		{
			world->update();
			Scene::getInstance()->render3D();
		};
		
		Script::updateAllTriggers();

		Scene::getInstance()->beginRender2D();

			menuRoot->render();
			helpNode->render();
			text->render();

		Scene::getInstance()->endRender2D();

		mouse.update();

		renderer->endRender( true, false );
	};
	
	delete GUINode::gRoot;

	Scene::getInstance()->destroy();

	delete world;

	delete renderer;
	
	return 0;
}