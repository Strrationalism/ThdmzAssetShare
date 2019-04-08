#version 110

uniform sampler2D tex;
uniform float time;
uniform float fogEnd;
uniform vec2 pointLight;
varying vec3 worldPos;
varying vec2 cameraPos;

vec4 GetFogedTexture(){
	float fogDensity = clamp(-worldPos.z,0.0,fogEnd) / fogEnd;
	
	fogDensity = fogDensity * fogDensity;
	vec4 texColor = texture2D(tex,gl_TexCoord[0].xy);
	
	return mix(texColor,vec4(0.0,0.0,0.0,1.0),fogDensity);
}

vec4 GetLightedTexture(vec4 color){
	float zDepth = 1.0-clamp(-worldPos.z / 2.0,0.0,1.0);
	vec2 pos = pointLight;
	pos.y = -pos.y;
	pos.x /= 1280.0;pos.y /= 720.0;
	float dis = 1.0-length(cameraPos - pos);
	return zDepth * dis * color * 2.0 * (0.15 * sin(time) + 0.85);
}

void main(){
	vec4 fogged = GetFogedTexture();
	vec4 color = GetLightedTexture(fogged);
	gl_FragColor = color * gl_Color;
}
