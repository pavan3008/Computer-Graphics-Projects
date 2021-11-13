
#version 130

out  vec3  vN;
out  vec3  vL;
out  vec3  vE;
out  vec2  vST;

uniform float	uTime;	
uniform bool vertPattern; //Controls the Vertex

const float PI = 	3.14159265;
const float AMP = 	0.2;		
const float W = 	2.;

// light position
vec3 LightPosition = vec3(  3., 5., 2. );


void
main( )
{ 
	
	//vertex coordinates
	vec3 vert = gl_Vertex.xyz;

	vec4 ECposition = gl_ModelViewMatrix * vec4( vert, 1. );

	vN = normalize( gl_NormalMatrix * gl_Normal );	
	vL = LightPosition - ECposition.xyz;			
	vE = vec3( 1., 1., 0. ) - ECposition.xyz;	
	
	// vertex shader animation
	if(vertPattern){
	vert.y = vert.y + tan(1.5 * PI * uTime);
	vert.x = vert.x * tan(uTime * 2 * PI);
	vert.z = vert.z + (uTime) * AMP;
	}

	vST = vert.xy; 
	gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}