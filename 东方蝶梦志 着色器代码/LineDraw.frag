#version 110
uniform sampler2D iChannel0;

float GetLight(vec2 texCoord,vec2 offset)
{
    vec3 col = texture2D(iChannel0,texCoord + offset).rgb;
    return col.r * 0.6 + col.g * 0.29 * col.b * 0.11;
}

float EdgeDeceting(vec2 texCoord,vec2 singlePixelSize,float texCC)
{
    
    float vLL = abs(GetLight(texCoord,singlePixelSize * vec2(-1,-1)) - texCC);
    float vLC = abs(GetLight(texCoord,singlePixelSize * vec2(-1,0)) - texCC);
    float vLR = abs(GetLight(texCoord,singlePixelSize * vec2(-1,1)) - texCC);
    
    float vCL = abs(GetLight(texCoord,singlePixelSize * vec2(0,-1)) - texCC);
    float vCR = abs(GetLight(texCoord,singlePixelSize * vec2(0,1)) - texCC);
   
    float vRL = abs(GetLight(texCoord,singlePixelSize * vec2(-1,-1)) - texCC);
    float vRC = abs(GetLight(texCoord,singlePixelSize * vec2(-1,0)) - texCC);
    float vRR = abs(GetLight(texCoord,singlePixelSize * vec2(-1,1)) - texCC);
    
    float sum = vLL + vLC + vLR + vCL + vCR + vRL + vRC + vRR;
    
    return sum;
}

void main()
{
	const vec2 iResolution = vec2(1024.0,768.0);
    vec2 single = vec2(1.0,1.0) / iResolution.xy;
	
	vec3 texel = texture2D(iChannel0,gl_TexCoord[0].xy).rgb;
    float texelLight = texel.r * 0.6 + texel.g * 0.29 + texel.b * 0.11;
    
    float edge = EdgeDeceting(gl_TexCoord[0].xy,single*1.5,texelLight);

    

    
    
    texelLight = texelLight *0.75 + 0.25;
    
    float finalCol = mix(texelLight,1.0-edge * 0.75,0.5);
   
    gl_FragColor = vec4(vec3(finalCol),1.0);
}
