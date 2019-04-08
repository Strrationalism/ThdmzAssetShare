#version 110

uniform sampler2D FB;


void main()
{
	vec4 col = texture2D(FB,gl_TexCoord[0].xy);
	
	float denAdd = (gl_TexCoord[0].y) * 0.15 * col.r;
	col += vec4(denAdd);
	
	gl_FragColor = col * gl_Color;
}
