#version 110
uniform sampler2D sourceTex;  
uniform sampler2D targetTex;  
uniform float time; // Ranges from 0.0 to 1.0  
  

void main()
{
    const float rollWidth = 5.0;
    float rollX = 1.0-time;
    
    
    vec2 texCoord = gl_TexCoord[0].xy;
    bool useLeft = texCoord.x > rollX;
    
    float dis = texCoord.x - rollX;
    
    float power = 1.0-clamp(rollWidth*abs(dis),0.0,1.0);
    power = power * power;
    
    texCoord.y -= 0.5;
    texCoord.y *= 1.0 + (1.0-1.0*cos(power));
    texCoord.y += 0.5;
    
    texCoord.x *= 1.0 - power*0.1;
    
    vec4 texel;
    if(useLeft)
        texel = texture2D(targetTex,texCoord);
    else
        texel = texture2D(sourceTex,texCoord);
    
    gl_FragColor = mix(vec4(vec3(0.0),1.0),texel,1.0 - abs(power));
}
