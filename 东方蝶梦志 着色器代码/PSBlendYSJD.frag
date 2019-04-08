#version 110

//PS 混合模式 颜色减淡

uniform sampler2D mainBG,blendBG;

void main()
{
	vec3 baseCol = texture2D(mainBG,gl_TexCoord[0].xy).rgb;
	vec3 blendCol = texture2D(blendBG,gl_TexCoord[0].xy).rgb;

	vec3 part;
	part.r = baseCol.r * blendCol.r;
	part.g = baseCol.g * blendCol.g;
	part.b = baseCol.b * blendCol.b;

	vec3 resultCol = baseCol + part / (vec3(1.0) - blendCol);

	gl_FragColor = vec4(resultCol,1.0);

}

