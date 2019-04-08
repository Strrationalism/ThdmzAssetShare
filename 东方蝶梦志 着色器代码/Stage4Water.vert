#version 110

varying float zDepth;
void main()
{
	gl_Position = ftransform();
	zDepth = gl_Vertex.z;
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = gl_Color;
}
