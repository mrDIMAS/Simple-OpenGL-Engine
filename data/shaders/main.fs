varying vec4 diffuse[ 8 ],ambientGlobal,ambient[ 8 ], ecPos;
varying vec3 normal,halfVector[ 8 ];
varying float dist;
 
 uniform sampler2D diffTex;
void main()
{
    vec3 n,halfV,viewV,lightDir;
    float NdotL,NdotHV;
    vec4 color = ambientGlobal;
    float att;
     
    /* a fragment shader can't write a verying variable, hence we need
    a new variable to store the normalized interpolated normal */
    n = normalize(normal);
    
    vec4 texColor = texture2D( diffTex,gl_TexCoord[0].st);

    for( int i = 0; i < 8; i++ )
    {
        // Compute the ligt direction
        lightDir = vec3(gl_LightSource[i].position-ecPos);
     
        /* compute the distance to the light source to a varying variable*/
        dist = length(lightDir);
 
     
        /* compute the dot product between normal and ldir */
        NdotL = max(dot(n,normalize(lightDir)),0.0);
 
        if (NdotL > 0.0) {
     
            att = 1.0 / (gl_LightSource[i].constantAttenuation +
                    gl_LightSource[i].linearAttenuation * dist +
                    gl_LightSource[i].quadraticAttenuation * dist * dist);
            color += att * (diffuse[ i ] * NdotL + ambient[ i ] );
     
         
            halfV = normalize(halfVector[ i ]);
            NdotHV = max(dot(n,halfV),0.0);
            color += att * gl_FrontMaterial.specular * gl_LightSource[i].specular * pow(NdotHV,gl_FrontMaterial.shininess);
    
        }
       
    }
    
    gl_FragColor = color * texColor;
}
