#version 110

varying vec2 orgPos;
void main(){
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_Position = ftransform();
	orgPos = gl_Vertex.st;
}
