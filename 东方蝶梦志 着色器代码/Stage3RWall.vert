#version 110
varying vec3 worldPos;
varying vec3 normal;
varying vec2 cameraPos;
void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
	worldPos = (gl_ModelViewMatrix * gl_Vertex).xyz;
	normal = normalize(gl_Normal* gl_NormalMatrix);
	gl_Position = ftransform();
	cameraPos = gl_Position.xy;
}
