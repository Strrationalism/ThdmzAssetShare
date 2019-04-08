#version 110

uniform vec2 center;
uniform float time;
uniform sampler2D fbo;

uniform int mode;	//0 - FullScreen
					//1 - HalfScreen

float Rand(vec2 co){
	return fract(sin(dot(co.xy, vec2(12.9898,78.233))) * 43758.5453);
}

float FlashPower(vec2 posCoord,float time,vec2 center)
{
    time = time * 1.25;
    
    float disToCenter = distance(center,posCoord);
    
    float clamperTop = time;
	if(mode == 0)
		clamperTop *= 3.0;
	
    float space = clamp(time * 1.1,0.0,1.0);
    float clamperBottom = clamp(time-space,0.0,1.1);
    
    if(disToCenter < clamperTop && disToCenter > clamperBottom)
    {
        float t = disToCenter - clamperBottom;
        t = t / (clamperTop - clamperBottom);
        
        t = clamp(t,0.0,1.0);

        if(t > 0.5)
            return 1.0 - t;
        return t;
    }
    return 0.0;
}

vec3 ChangeCol(vec3 col,vec2 texCoord)
{
    //return col;
	//texCoord = floor(texCoord * 50.0) / 50.0;
	//vec3 mainCol = texture2D(fbo,vec2(1.0) - texCoord).rgb;
    return col * 2.0;
}

void main()
{   
    //transform fragCoord
	vec2 texCoord = gl_TexCoord[0].xy;
    vec2 posCoord = vec2(gl_TexCoord[0].x * 1.77777777,gl_TexCoord[0].y);


    float power = FlashPower(posCoord,time,center) * 1.5;
	power = power * (1.0-time);
    

    vec2 f = floor(posCoord * 50.0) / 50.0;
    vec2 rnd;
    rnd.x = 4.0*Rand(f) * power * 0.1;
    rnd.y = 4.0*Rand(vec2(f.y,f.x)) * power * 0.1;
    vec3 col = texture2D(fbo,texCoord + rnd).rgb;
    
    col = mix(col,ChangeCol(col,texCoord),power) ;//* (1.0 + 2.0*power);
    
    
    gl_FragColor = vec4(col,1.0);
}
