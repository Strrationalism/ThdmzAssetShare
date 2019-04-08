#version 110

uniform sampler2D texture;
uniform float time,mul,heightMul,height;

void main(){
	float h = sin(time + (mul * gl_TexCoord[0].s)*3.14) * 0.5 * heightMul + height;
	gl_FragColor = texture2D(texture,gl_TexCoord[0].st);
	
	gl_FragColor.a *= (clamp(gl_TexCoord[0].t,h,h+0.08) - h)/0.08;
	
}
