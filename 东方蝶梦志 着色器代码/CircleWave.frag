#version 110

uniform float time;

uniform sampler2D tex;
uniform float progress;

const float radius = 0.45;

float GetDistance(){
	vec2 l = (gl_TexCoord[0].st - vec2(0.5,0.5));
	float len = sqrt(abs(l.x*l.x + l.y*l.y));
	if(len >= radius) len = radius;
	return len;
}

void main() {
	float distance = GetDistance();
	float ampFactor = progress * (radius-distance) * 0.08;

	vec2 coord = gl_TexCoord[0].st;
    vec2 dis = (coord - vec2(0.5,0.5)) * 2.0;  
     
    coord += normalize(dis) * cos(time + length(dis) * 8.0) * ampFactor;  
      
	if(distance >= radius) gl_FragColor = vec4(0,0,0,0);
	else gl_FragColor = texture2D(tex, coord);
}  
