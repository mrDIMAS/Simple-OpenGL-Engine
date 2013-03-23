#include "TextNode.h"

TextNode::TextNode( Font * font, int x, int y, const char * text, int rw, int rh )
{
	this->font	= font;		
	kerning		= 0.7;
	setPosition( x, y );
	setText( text );
	rectW = rw;
	rectH = rh;
};

void TextNode::setKerning( float k )
{
	kerning = k;

	w = text.size() * font->getRasterCellSize() * kerning;
	h = font->getRasterCellSize();
};

void TextNode::setText( const string & txt )
{
	text = txt;
	w = ( text.size() )* font->getRasterCellSize() * kerning;
	h = font->getRasterCellSize() ;
};

void TextNode::setTextEx( const char * txt, ... )
{
	va_list	argumentList;
	char buffer[512];	

	va_start( argumentList, txt );

	vsprintf_s( buffer, txt, argumentList );

	va_end( argumentList );

	text = buffer;
};

void TextNode::onRender( )
{
	int ty = y;
	int symbCountOnLine = 0;

	for( uint i = 0; i < text.size(); i++ )
	{	
		int symbol = static_cast<unsigned char>( text[ i ] );

		if( symbol == '_' )
			symbol = ' ';

		if( symbol == '\n' )
		{
			ty += scalery * font->getRasterCellSize();

			symbCountOnLine = 0;

			continue;
		}
			
		if( symbol == '\t' )
		{
			symbCountOnLine += 4;

			continue;
		}

		int gx = x + symbCountOnLine * ( scalerx * font->getRasterCellSize() * kerning );

		if( rectW )
			if( gx > ( x + rectW ) )
			{
				ty += scalery * font->getRasterCellSize();

				symbCountOnLine = 0;
			}

		font->drawSymbol( x + symbCountOnLine * ( scalerx * font->getRasterCellSize() * kerning ), ty , symbol, scalerx, scalery );

		symbCountOnLine++;
	};	
};