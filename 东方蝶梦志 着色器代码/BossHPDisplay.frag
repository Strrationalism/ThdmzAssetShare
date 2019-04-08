#version 110
uniform float alpha;
uniform float m_hp;
varying float per;

void main(){
	if(m_hp >= 1.0)
		gl_FragColor = vec4(vec3(1.0),1.0);
	else
	{
		const float fadePower = 0.05;
		float whitePower = (clamp(m_hp - per,-fadePower,fadePower) + fadePower) / fadePower * 2.0;
		vec3 col = vec3(whitePower);
		if(m_hp <= fadePower) {
			col *= m_hp / fadePower;
		}
		gl_FragColor = vec4(col,alpha);
	}
}
