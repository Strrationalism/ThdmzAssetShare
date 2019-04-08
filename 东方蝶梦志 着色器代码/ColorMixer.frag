#version 110

uniform float delta;
uniform sampler2D tex;
void main()
{
    vec4 texel = texture2D(tex,gl_TexCoord[0].xy);
    vec3 col = mix(gl_Color.rgb,texel.rgb,delta);
    gl_FragColor = vec4(col,texel.a) * gl_Color.rgba;
}
