#pragma once

#include "Common.h"


 
static char * getFileContent(const char *path)
{
	FILE *fd;
	long len, r;
	char *str;
 
	if (!(fd = fopen(path, "r")))
	{
		fprintf(stderr, "Can't open file '%s' for reading\n", path);
		return NULL;
	}
 
	fseek(fd, 0, SEEK_END);
	len = ftell(fd) + 1;
 
	printf("File '%s' is %ld long\n", path, len);
 
	fseek(fd, 0, SEEK_SET);
 
	if (!(str = (char*)malloc(len * sizeof(char))))
	{
		fprintf(stderr, "Can't malloc space for '%s'\n", path);
		return NULL;
	}
 
	memset( str, 0, len );

	r = fread(str, sizeof(char), len, fd);
 
	str[len - 1] = '\0'; /* Shader sources have to term with null */
 
	fclose(fd);
 
	return str;
}


class Shader
{
protected:

	uint createShader( const char * file, uint type )
	{
		uint shader_id = 0;
		// skip error code
		glGetError( );
		// vertex shader
		char * shader_data = getFileContent( file ); // read data from file
		if( !shader_data )
		{
			printf( "unable to create shader! bad file" );
			return 0;
		}
		shader_id = glCreateShader( type );
		if( !shader_id )
		{
			printf( "unable to create shader object!" );
			return 0;
		}
		const char * c_ptr_data = shader_data;
		glShaderSource( shader_id, 1, &c_ptr_data, 0 );
		free( shader_data ); // free allocated memory
		if( glGetError( ) != GL_NO_ERROR )	
		{
			printf( "unable to set shader data!" );
			return 0;
		}
		glCompileShader( shader_id );
		if( glGetError( ) != GL_NO_ERROR )	
		{
			printf( "unable to compile shader!" );
			return 0;
		}
		// read shader compilation log
		const uint shader_log_size = 4096;
		char shader_log[ shader_log_size ] = { 0 };
		GLsizei out_log_lenght = 0;
		glGetShaderInfoLog( shader_id, shader_log_size, &out_log_lenght, shader_log );
		printf( "shader compilation log: \n %s", shader_log );
		return shader_id;
	}
public:
	uint vertex_shader_id;
	uint fragment_shader_id;
	uint program_id;

	Shader( const char * vertexShader, const char * fragmentShader )
	{
		vertex_shader_id = createShader( vertexShader, GL_VERTEX_SHADER );
		fragment_shader_id = createShader( fragmentShader, GL_FRAGMENT_SHADER );
		if( !(vertex_shader_id && fragment_shader_id) ) 
		{
			printf( "unable to create shader at all :)" );
			return;
		}
		// program
		program_id = glCreateProgram( );
		glAttachShader( program_id, vertex_shader_id );
		glAttachShader( program_id, fragment_shader_id );
		glLinkProgram( program_id );	
	};

	~Shader( )
	{
		Shader::unbind();
		glDeleteProgram( program_id );
		glDeleteShader( vertex_shader_id );
		glDeleteShader( fragment_shader_id );		
	};

	void bind( )
	{
		glUseProgram( program_id );
	};

	static void unbind( )
	{
		glUseProgram( 0 );
	};
};

