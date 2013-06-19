varying vec4 diffuse[ 8 ],ambientGlobal,ambient[ 8 ], ecPos;
varying vec3 normal,halfVector[ 8 ];
varying float dist;
 
void main()
{  
    vec3 aux;
     
    /* first transform the normal into eye space and normalize the result */
    normal = normalize(gl_NormalMatrix * gl_Normal);
 
    /* compute the vertex position  in camera space. */
    ecPos = gl_ModelViewMatrix * gl_Vertex;
 
	for( int i = 0; i < 8; i++ )
	{
		/* Normalize the halfVector to pass it to the fragment shader */
		halfVector[ i ] = gl_LightSource[i].halfVector.xyz;
     
		/* Compute the diffuse, ambient and globalAmbient terms */
		diffuse[ i ] = gl_FrontMaterial.diffuse * gl_LightSource[i].diffuse;
		ambient[ i ] = gl_FrontMaterial.ambient * gl_LightSource[i].ambient;
	}

    ambientGlobal = gl_LightModel.ambient * gl_FrontMaterial.ambient;
     
         
gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_Position = ftransform();
} 