
#version 110

uniform sampler2D texture;

uniform vec2 circleCenter;
uniform float circleRadius2;

varying vec2 orgPos;	//未变换的二维坐标
 
void main() {
   gl_FragColor = texture2D(texture,gl_TexCoord[0].st); 
   
   //如果在圆的范围之内
   float len = length(orgPos - circleCenter);
	vec4 negaColor = vec4(1,1,1,2)- gl_FragColor;
	float clen = (clamp(len,circleRadius2,circleRadius2 + 20.0) - circleRadius2) / 20.0;
	clen = sin(clen * 3.1415926/2.0);
	gl_FragColor = mix(gl_FragColor,negaColor,clen);
	gl_FragColor.a = 1.0;
}
