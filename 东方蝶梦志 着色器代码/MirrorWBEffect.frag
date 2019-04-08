#version 110

uniform vec2 PlayerPoint;
uniform sampler2D Frame;

vec3 downRGBSub(vec3 color,float power)
{
    vec3 wb = vec3(color.r * 0.3 + color.g * 0.6 + color.b * 0.1);
    return mix(color,wb,power);
}

float getPower(vec2 center,vec2 thisPoint)
{
    return smoothstep(8.0*distance(center,thisPoint),0.0,1.0);
}

void main() 
{
    vec2 coord = gl_TexCoord[0].xy;
    const float screenWHdiv = 16.0 / 9.0;
    vec2 center = PlayerPoint;
    vec2 transformedCenter = vec2(screenWHdiv * center.x,center.y);
    vec2 transformedThis = vec2(screenWHdiv * coord.x,coord.y);
    

    float power = getPower(transformedCenter,transformedThis);
    
    vec4 color = texture2D(Frame,coord);
    vec3 col = downRGBSub(color.rgb,power) * (0.9 * (1.0-power) + 0.1);
  	gl_FragColor = vec4(col,1.0);
}
