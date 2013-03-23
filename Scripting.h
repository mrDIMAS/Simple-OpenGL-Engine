#pragma once

#include <unordered_map>
#include <iostream>
#include <string>
#include <conio.h>
#include <fstream>

using namespace std;

namespace Dynamic
{
	class Value
	{
		string value;
	public:
		Value( )
		{

		};

		Value( string & val )
		{
			value = val;
		};

		Value( const char * val )
		{
			value = val;
		};

		float getNum( )
		{
			return atof( value.c_str() );
		};

		string getStr( )
		{
			return value;
		};
	
		void set( string & val )
		{
			value = val;
		};

		void set( float num )
		{
			char buffer [ 128 ] = { 0 };

			sprintf( buffer, "%f", num );

			value = buffer;
		};

		operator string( )
		{
			return value;
		};

		operator const char * ( )
		{
			return value.c_str();
		};

		operator float () 
		{
			return getNum();
		};
	};

	namespace Parser
	{
		typedef unordered_map<string, Value> ValueMap;

		static void parseValues( const string & str, ValueMap & variables )
		{
			if( str.size() <= 1 )
				return;

			variables.clear();

			bool equalFound = false;
			bool quoteLF = false, quoteRF = false;
			int n = 0;

			string varName, var;

			while( true )
			{
				char symbol = str.at( n );

				if( symbol == ';' )
				{
					quoteLF		= false;
					quoteRF		= false;
					equalFound	= false;

					varName.clear();
					var.clear();
				};
			
				if( isalpha( (unsigned char)symbol ) || isdigit( (unsigned char)symbol ) || symbol == '_' )
				{
					if( !equalFound )
					{
						varName.push_back( symbol );
					}
				}
				else
				{
					if( symbol == '=' )
						equalFound = true;

					if( symbol == '"' )
					{
						if( quoteLF == false )
						{
							quoteLF = true;
						}
						else
						{
							quoteRF = true;
						}
					}
				};

				if( quoteLF )
				{
					if( quoteRF )
					{
						variables[ varName ] = Value( var );
					}
					else
					{
						if( symbol != '"' )
							var.push_back( symbol );
					};
				};

				n++;

				if( n >= str.size( ) )
					break;
			};
		}

		static void parseFile( const char * fileName, ValueMap & variables )
		{
			FILE * file = fopen( fileName, "r" );

			if( !file )
				return;
			
			string str;

			while( !feof( file ))
			{
				char symbol = '\0'; fread( &symbol, sizeof( char ), 1, file );
				str.push_back( symbol );
			};

			fclose ( file );

			if( str.size() <= 1 )
				return;

			variables.clear();

			bool equalFound = false;
			bool quoteLF = false, quoteRF = false;
			int n = 0;

			string varName, var;

			while( true )
			{
				char symbol = str.at( n );

				if( symbol == ';' )
				{
					quoteLF		= false;
					quoteRF		= false;
					equalFound	= false;

					varName.clear();
					var.clear();
				};
			
				if( isalpha( (unsigned char)symbol ) || isdigit( (unsigned char)symbol ) || symbol == '_' )
				{
					if( !equalFound )
					{
						varName.push_back( symbol );
					}
				}
				else
				{
					if( symbol == '=' )
						equalFound = true;

					if( symbol == '"' )
					{
						if( quoteLF == false )
						{
							quoteLF = true;
						}
						else
						{
							quoteRF = true;
						}
					}
				};

				if( quoteLF )
				{
					if( quoteRF )
					{
						variables[ varName ] = Value( var );
					}
					else
					{
						if( symbol != '"' )
							var.push_back( symbol );
					};
				};

				n++;

				if( n >= str.size( ) )
					break;
			};
		}

		static void parseFunction( const string & str, string & funcName, vector<string> & args )
		{
			if( str.size() <= 1 )
				return;

			funcName.clear();
			args.clear();

			bool equalFound = false;
			bool bracketLF = false;
			bool quoteLF = false, quoteRF = false;
			int n = 0;

			string arg;

			while( true )
			{
				int symbol = str.at( n );

				if( quoteRF )
				{
					if( symbol == ',' || symbol == ')' )
					{
						args.push_back( arg );

						arg.clear();

						quoteLF = 0;
						quoteRF = 0;
					};
				}

				if( symbol == ')' )
					break;

				if( symbol == '(' )
					bracketLF = true;

				if( symbol == '<' )
				{
					quoteLF = true;
				}

				if( symbol == '>' )
				{
					quoteRF = true;
				}

				if( ( isalpha( (unsigned char)symbol ) || isdigit( (unsigned char)symbol ) || symbol == '_' ) && symbol != '<' && symbol != '>' )
				{
					if( !bracketLF )
					{
						funcName.push_back( symbol );
					}
				};

				if( symbol != '<' && symbol != '>' )
				{
					if( bracketLF )
					{
						if( quoteLF )
							arg.push_back( symbol );
					}
				};

				n++;

				if( n >= str.size( ) )
					break;
			};
		}
	}

	namespace Function
	{
		typedef vector<string> ArgumentList;

		static unordered_map< string, void (*)( ArgumentList & args )> functions;

		static void Register( string name, void (*func)( ArgumentList & args ) )
		{
			functions[ name ] = func;

			cout << "Function registered: " << name << endl;
		};

		static void Call( string name, ArgumentList & args )
		{
			auto func = functions.find( name );

			if( func != functions.end() )
			{
				func->second( args );
			}
			else
			{
				cerr << "Err: Undeclared function call: " << name;

				for( unsigned int i = 0; i < args.size(); i++ )
				{
					if( i == 0 )
						cerr << "(";

					cerr << args.at( 0 );

					if( i == args.size() - 1 )
						cerr << ")" << endl;
					else
						cerr << ", ";
				};
			
			};
		};
	};
}