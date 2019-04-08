#version 110

//0,1 - vec2 x,y'
//2 - angle
//3 - size
attribute vec4 attr;

vec2 RotateVec2_2D() {
		//cos(a) sin(a)
		//-sin(a) cos(a)
		float angle = attr[2] * 3.141592653589793238462643383 / 180.0;
		return vec2(
			gl_Vertex.x * cos(angle) - gl_Vertex.y * sin(angle),
			gl_Vertex.x * sin(angle) + gl_Vertex.y * cos(angle)
		);
}

void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	
	
	
	gl_Position = gl_ProjectionMatrix * vec4(( (RotateVec2_2D() * attr[3]*1.5) + attr.xy),0,1);
	//
}
