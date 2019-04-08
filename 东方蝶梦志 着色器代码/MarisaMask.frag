#version 110

uniform sampler2D tex;
uniform sampler2D mask;

uniform float time;

void main() {
	vec4 mask = texture2D(mask,gl_TexCoord[0].xy);
	vec4 texColor = texture2D(tex,vec2(gl_TexCoord[0].x + time,gl_TexCoord[0].y));
	vec4 texColor2 = texture2D(tex,vec2(gl_TexCoord[0].x - time,1.0-gl_TexCoord[0].y));
	
	gl_FragColor = mask * (texColor + texColor2) * gl_Color;
	gl_FragColor.rgb *= 1.255;
}  
