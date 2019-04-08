#version 110

uniform sampler2D FB;


vec3 Light(float den,vec3 lightColor)
{
	return vec3(lightColor) * (1.0-0.4*den);
}


vec3 Shadow(vec3 col)
{
	float t = gl_TexCoord[0].y;
	float gray = t * 0.9 + 0.1;
	gray *= 2.0;
	
	gray = clamp(gray,0.0,1.0);
	return vec3(gray) * col;
}

void main()
{
	float den = texture2D(FB,gl_TexCoord[0].xy).r;
	
	vec3 col = Light(den,gl_Color.rgb);
	col = Shadow(col);
	
	gl_FragColor = vec4(col,den*4.0);
}
