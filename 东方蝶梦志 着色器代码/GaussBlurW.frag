#version 110

uniform sampler2D fb;
uniform vec2 resolution;
uniform float radius;

float Gaussian(float x)
{
	const float sigma = 1.0;
	float part1 = 1.0/sqrt(2.0*3.1415926)*sigma;
	
	const float miu = 0.0;
	float part2 = -(pow(x - miu,2.0)/2.0*sigma*sigma);
	return part1*exp(part2);
}

vec4 sample(float weight,float pointOffset)
{
	vec2 samplePoint = vec2(pointOffset/resolution.y,0.0) + gl_TexCoord[0].xy;
	vec4 col = texture2D(fb,samplePoint);
	return col * weight;
}

void main()
{
	vec4 col = vec4(0.0);
	float weightSum = 0.0;
	
	for(float offset = -radius;offset <= radius;offset = offset + 0.5)
	{
		float weight = Gaussian(offset / radius);
		col += sample(weight,offset);
		weightSum += weight;
	}
	
	col /= weightSum;
	
	gl_FragColor = vec4(col) * gl_Color;
}
