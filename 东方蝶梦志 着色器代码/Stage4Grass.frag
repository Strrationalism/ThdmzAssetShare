#version 110

uniform sampler2D screenShot;
uniform sampler2D grassImage;
uniform float depth;

void main()
{
	vec3 uvOffset = texture2D(grassImage,gl_TexCoord[0].xy).xyz - vec3(0.5,0.5,0.0);
	uvOffset *= 0.25;
	uvOffset *= depth;
	
	uvOffset.xy -= vec2(0.15) * uvOffset.z;
		
	vec4 colA = texture2D(screenShot,gl_TexCoord[0].xy + uvOffset.xy);
	gl_FragColor = colA;
}
