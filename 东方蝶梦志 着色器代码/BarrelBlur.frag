#version 110
uniform sampler2D tex;  
uniform float barrelPower;
uniform vec2 center;
  
void main()
{
    //Centers
    //vec2 center = iMouse.xy / iResolution.xy;
    //const float barrelPower = 1.0;
    vec2 uv = gl_TexCoord[0].xy;
    
    //Direction To Center
    vec2 directionToCenter = center - uv;
    float distanceToCenter = length(directionToCenter);
    directionToCenter = sin(normalize(directionToCenter));
    
    vec4 col = vec4(0.0);
    
    for(float i = 0.0;i < 15.0;++i)
    {
        vec2 curUv = uv + directionToCenter * distanceToCenter * 0.005 * i * barrelPower;
        col.r += texture2D(tex,curUv).r;
        
        curUv = uv + directionToCenter * distanceToCenter * 0.01 * i * barrelPower;
        col.g += texture2D(tex,curUv).g;
        
        curUv = uv + directionToCenter * distanceToCenter * 0.015 * i * barrelPower;
        col.b += texture2D(tex,curUv).b;
    }
    
    
    gl_FragColor = col / 15.0;
}
