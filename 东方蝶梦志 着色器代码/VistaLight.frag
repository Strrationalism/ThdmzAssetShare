#version 110
uniform sampler2D cloud;
uniform float time;

float Perlin(vec2 coord)	//采样柏林噪声
{
	vec4 f = texture2D(cloud,coord);
	return f.a;
}

float LineHeight()	//获取极光当前Y上的线条的高度
{
	vec2 coord = vec2(time / 500.0,gl_TexCoord[0].x);
	return Perlin(coord);
}

float LineHeight2()	//获取极光当前Y上的线条的高度
{
	vec2 coord = vec2(-time / 500.0,1.0-gl_TexCoord[0].x);
	return Perlin(coord);
}

float Rd()	//获取极光当前Y上的线条的高度
{
	vec2 coord = vec2(time / 100.0,1.0-gl_TexCoord[0].x);
	return Perlin(coord);
}

float Gen2DLight(float height,float y,float rd)	//获取2D面上，极光线条发散出来的光的亮度
{
	//height	-	极光线条高度
	//y			-	当前点高度
	//rd		-	对当前位置亮度的扰动
	
	float len = 1.0 - (height - y) * (4.0 - (rd * 0.9));
	if(len > 1.0)
	{
		len = clamp((1.2 - len),0.0,0.2);
		len *= 5.0;
	}
	else
	{
		len = clamp(len,0.0,1.0);
	}
	return len;
}

vec3 GetColor(float yOffset)
{
	float light = cos(gl_TexCoord[0].x * 4.0 - 2.0);
	vec2 off = vec2(0.0,yOffset);
	float r = Perlin(gl_TexCoord[0].xy + off);
	float g = Perlin(gl_TexCoord[0].yx + off);
	float b = Perlin(off + (vec2(1.0) - gl_TexCoord[0].xy));
	
	return (vec3(r,g,b) / 2.0 + vec3(0.5)) * light;
}

vec3 Calc1()
{
	float lineHeight = LineHeight();
	float light	= Gen2DLight(lineHeight*0.75 - 0.1,gl_TexCoord[0].y,Rd());
	
	const vec3 mainTheme = vec3(0.407843137254902,0.992156862745098,0.890196078431372);
	vec3 col = vec3(GetColor(lineHeight) * light);
	return col * mainTheme;
}

vec3 Calc2()
{
	float lineHeight = LineHeight2();
	float light	= Gen2DLight(lineHeight*0.5,gl_TexCoord[0].y,Rd());
	
	const vec3 mainTheme = vec3(0.968627450980392,0.682352941176471,0.992156862745098);
	vec3 col = vec3(GetColor(lineHeight) * light);
	return col * mainTheme;
}

void main()
{	
	vec3 col1 = Calc1() + Calc2();
	gl_FragColor = vec4(col1,gl_Color.a);
}
