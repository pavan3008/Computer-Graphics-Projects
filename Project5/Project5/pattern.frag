#version 130

uniform float uKa, uKd, uKs;
uniform float uS0, uT0;
uniform vec3  uColor;
uniform vec3  uSpecularColor;
uniform float uShininess;
uniform float uSize;
uniform float uTime;
uniform bool fragPattern; // control the fragment

const float PI = 	3.14159265;
const float AMP = 	1.0;	
const float W = 	2.;
in  vec2  vST;
in  vec3  vN;
in  vec3  vL;
in  vec3  vE;	

void
main( )
{
	vec3 Normal 	= normalize(vN);
	vec3 Light 		= normalize(vL);
	vec3 Eye 		= normalize(vE);
	vec3 myColor 	= uColor;

	float frag1 = tan( uTime * PI);
	float frag2 = cos( uTime * AMP);

	if(fragPattern){
		if (vST.t < sin(W * (vST.s * frag1 * 30)))
			myColor = vec3( tan(5. * uTime), frag1/2, frag2/2 );
		//else
			//myColor = vec3( uColor.x, uColor.y, frag2/3);
	}


	vec3 ambient = uKa * myColor;

	float d = max( dot(Normal,Light), 0. );
	vec3 diffuse = uKd * d * myColor;

	float s = 0.;
	if( dot(Normal,Light) > 0. )
	{
		vec3 ref = normalize(  reflect( -Light, Normal )  );
		s = pow( max( dot(Eye,ref),0. ), uShininess );
	}
	//vec3 specular = uKs * s * uSpecularColor;
	gl_FragColor = vec4( ambient + diffuse,  1. ); // not used specular for better visualization
}
#pragma once
