#version 110

uniform float Time;	//时间

uniform sampler2D Cloud;


vec2 Pos;	//位置


//计算形状
float Shape(vec2 pos){
	return clamp( sin(pos.x*3.1415926) - pos.y, 0.0, 1.0 );
}

float GetNoise( in vec3 x ) // iq noise function
{
	vec3 p = floor(x);
    vec3 f = fract(x);
	f = f*f*(3.0-2.0*f);
	vec2 uv = (p.xy+vec2(37.0,17.0)*p.z) + f.xy;
	vec2 rg = vec2(texture2D( Cloud, (uv+ 0.5)/16.0, 0.0 ).a);
	return mix( rg.x, rg.y, f.z ) * 2.0 - 1.0;
}

void main(){
	Pos = gl_TexCoord[0].xy;
	
	float time = Time * 0.75;
	
	vec2 posOffset = vec2(0.0,0.0);
	
	for(int i = 0;i < 10;++i){
		float ii = pow(float(i),2.0);
		float ifrac = float(i) / 25.0;
		float t = ifrac * time;
		float d = (1.0-ifrac) * 0.05;
		posOffset.x += GetNoise( vec3(Pos.x*ii-time*ifrac, 0.25*Pos.y*ii-t, 0.0)) * d * 2.0;
		posOffset.y += GetNoise( vec3(Pos.x*ii+time*ifrac, 0.25*Pos.y*ii-t, time*ifrac/ii)) * d;
	}
	
	vec2 ePos = posOffset + Pos;
	
	float shape = Shape(ePos) * 4.0;
	
	gl_FragColor = vec4(0.5,0.25,1.0,1.0) * shape;
	gl_FragColor *= gl_Color;
}
