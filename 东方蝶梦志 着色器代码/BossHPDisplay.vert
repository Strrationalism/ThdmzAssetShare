#version 110


const float CIRCLE_POINTS = 128.0;
const float M_PI = 3.14159265358979323846264;

varying float per;

void main(){
	float i = gl_Vertex[0];
	vec4 vert = vec4(-128.0 * sin(2.0 * M_PI / CIRCLE_POINTS * i),-128.0 * cos(2.0*M_PI / CIRCLE_POINTS * i),-1.0,1.0);

	per = i / CIRCLE_POINTS;
	gl_Position = gl_ModelViewProjectionMatrix * vert;
}
