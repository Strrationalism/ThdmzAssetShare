
#version 110

uniform sampler2D texture;

uniform vec2 circleCenter;
uniform float circleRadius2;

varying vec2 orgPos;	//未变换的二维坐标
const float cb = 2.0*100.0;
const float cc = 2.0*141.42135623730950488016887242097;
const float cirRadMul = 1.2;

void Circle(vec2 cb,float radius){
	vec2 circleCenterUsing = circleCenter - cb;
	float len = length(orgPos - circleCenterUsing);
		
	vec4 negaColor = vec4(1,1,1,2)- gl_FragColor;
	float clen = (clamp(len,radius,radius + 10.0) - radius) / 10.0;
	clen = cos(clen * 3.1415926/2.0);
	gl_FragColor = mix(gl_FragColor,negaColor,clen);
}
 
void main() {
   gl_FragColor = texture2D(texture,gl_TexCoord[0].st); 
   
   float rad = cirRadMul * circleRadius2;
   
   //如果在圆的范围之内
   //左上方圆
	Circle(vec2(cb,cb),rad);
		
	//左下方圆
	Circle(vec2(cb,-cb),rad);
	
	//右上方圆
	Circle(vec2(-cb,cb),rad);
	
	//右下方圆
	Circle(vec2(-cb,-cb),rad);
	
	Circle(vec2(0.0,cc),rad);
	Circle(vec2(0.0,-cc),rad);
	Circle(vec2(-cc,0.0),rad);
	Circle(vec2(cc,0.0),rad);
		
	Circle(vec2(0.0,0.0),rad);
	
	gl_FragColor.a = 1.0;
}
