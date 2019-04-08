#version 110

uniform sampler2D tex;
uniform float lmt;

void main()
{
	vec4 texel = texture2D(tex,gl_TexCoord[0].xy);
    float power = texel.r;
    vec3 col;   
    if(power > lmt)
    	col = mix(gl_Color.rgb,vec3(1.0),(power - lmt)/lmt);
    else
        col = mix(vec3(0.0),gl_Color.rgb,power/lmt);
    
    gl_FragColor = vec4(col,texel.a * gl_Color.a);
}
