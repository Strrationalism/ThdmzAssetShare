#version 110

uniform vec2 centerpos;
uniform float GlowRange;
uniform float lowestLum;
uniform sampler2D CC_Texture0;

vec4 GetLum(vec4 color){
	float lum = 0.3 * color.r + 0.59 * color.g + 0.11 * color.b;
	if(lum <= lowestLum/1.05) lum = 0.0;
	return vec4(lum,lum,lum,1.0);
}

//将坐标系缩放到16:9
vec2 ScalePos(vec2 scaleCenter,vec2 pos)
{
	vec2 posInCenter = pos - scaleCenter;
	posInCenter.x *= 16.0;
	posInCenter.x /= 9.0;
	return posInCenter + scaleCenter;
}

void main()                      
{    
	vec2 v_texCoord = ScalePos(centerpos,gl_TexCoord[0].xy);

	
    vec4 clraverge=vec4(0.0,0.,0.0,.0);   
    float range=GlowRange,count=0.,x1,y1;
	vec2 cpos=centerpos;    
	
	
	float len = distance(v_texCoord,cpos);
	
	float alp = 1.1 * len;
	if(len >= 0.7){
		discard;
	}
    for( float j = 1.; j<=range ; j += 2.9 )  
	{
		//起始点 cpos
		//终结点 v_texCoord
		//插值点 cpos ~ v_texCoord (j/range)
		vec2 texPos = mix(cpos,gl_TexCoord[0].xy,j/range);
		
	
		clraverge+=GetLum(texture2D( CC_Texture0,texPos));// vec2(x1,y1) );
		count+=1.;
	}
	clraverge/=count;
	
	clraverge *= 4.0;
	clraverge *= 1.1 * len;// * vec4(1.0,0.0,0.0,1.0);
	
	if(len >= 0.25){
		//距离在0.25到0.7之间，进行衰减
		float cv = (len-0.25)*2.2222;	//在0~1之间
		cv = 1.0-cv;
		//cv=-cv*cv+2.0*cv;
		//cv = sin(3.1415926/2.0*cv);
		clraverge *= cv;
	}
	
	clraverge *= gl_Color;
	
	gl_FragColor = clraverge;
}
