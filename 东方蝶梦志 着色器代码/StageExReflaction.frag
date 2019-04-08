#version 110

varying float zDepth;
uniform sampler2D tex;
uniform sampler2D waterImage;
uniform float time;

void main()
{
	float noiseMul = 0.25 + 0.25 * -zDepth;
	
	vec4 waterTexel = texture2D(waterImage,gl_TexCoord[0].xy*noiseMul + time / 50.0);
	vec4 waterTexel2 = texture2D(waterImage,-gl_TexCoord[0].xy*noiseMul - time / 50.0);
	vec2 uvOffset = waterTexel.gb + waterTexel2.gb;
	uvOffset /= 2.0;
	
	uvOffset *= -zDepth;
	
	vec4 bkColor = vec4(1.0) * zDepth / 2.0;
	
	vec2 texCoord = vec2(gl_TexCoord[0].x,gl_TexCoord[0].y / 16.0);
	vec4 col = texture2D(tex, texCoord + uvOffset);
	vec4 bcol = bkColor + col;
	float light = bcol.r * 0.39 + bcol.g * 0.5 + bcol.b * 0.11;
	gl_FragColor = vec4(vec3(light),1.0);
	//gl_FragColor = vec4(noiseMul);
}
