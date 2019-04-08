#version 110

uniform float t,alpha;
uniform sampler2D clouds;

const vec3 KeyColor1 = vec3(1.0,1.0,1.0);
const vec3 KeyColor2 = vec3(0.541176470588235,0.443137254901961,0.76078431372549);
const vec3 KeyColor3 = vec3(0.305882352941176,0.137254901960784,0.666666666666667);
const vec3 KeyColor4 = vec3(0.0,0.0,0.0);

vec3 mcol(float t1,float t2,vec3 col1,vec3 col2,float t)
{
	return mix(col1,col2,(t-t1)/(t2-t1));
}

void main()
{
	float colPower1 = texture2D(clouds,gl_TexCoord[0].xy + vec2(t*0.1,t)).r;
	float colPower2 = texture2D(clouds,vec2(0.5,0.25) + 0.95*gl_TexCoord[0].xy + vec2(t*0.05,t*0.5)).r;
	float colPower4 = texture2D(clouds,vec2(0.125,0.125) + 0.85*gl_TexCoord[0].xy + vec2(t*0.025,t*0.25)).r;

	float colPower = colPower1 + colPower2 + colPower4;
	colPower /= 3.0;

	colPower = clamp(colPower * colPower * 1.5,0.0,1.0);
	
	vec3 col;

	if(colPower > 0.66 && colPower <= 1.0)
		col = mcol(0.66,1.0,KeyColor2,KeyColor1,colPower);
	else if(colPower > 0.33 && colPower <= 0.66)
		col = mcol(0.33,0.66,KeyColor3,KeyColor2,colPower);
	else
		col = mcol(0.0,0.33,KeyColor4,KeyColor3,colPower);

	col *= 0.72;

	float whitePower = clamp(1.5*gl_TexCoord[0].y - 0.5,0.0,1.0);
	whitePower = whitePower * whitePower;
	col = mix(col,vec3(1.0),whitePower * 0.63);

	col = mix(col * 0.15,col,gl_TexCoord[0].y * 0.9);

	gl_FragColor = vec4(col * alpha,1.0);
}
