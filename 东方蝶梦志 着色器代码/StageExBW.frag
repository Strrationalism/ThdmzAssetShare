#version 110

uniform sampler2D tex;

void main()
{
	vec4 col = texture2D(tex,gl_TexCoord[0].xy);
	float light = col.r * 0.39 + col.g * 0.5 + col.b * 0.11;
	gl_FragColor = vec4(vec3(light),col.a);
}

