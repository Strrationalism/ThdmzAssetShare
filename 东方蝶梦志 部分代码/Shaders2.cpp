#include "NuzPrecompiled.h"
#include "Shaders2.h"
#include "GLShader.h"

using namespace Basic::Video::LowerRenderer;

static GLShader titleClouds;
static GLuint titleClouds_t, titleClouds_alpha;
void Basic::Shaders::TitleClouds(float t,float alpha)
{
	titleClouds.Use();
	glUniform1f(titleClouds_t, t);
	glUniform1f(titleClouds_alpha, alpha);
}

static GLShader psBlendYSJD;
void Basic::Shaders::PSBlendYSJD()
{
	psBlendYSJD.Use();
}

static GLShader colorConverter;
static GLuint colorConverter_lmt;
void Basic::Shaders::ColorConverter(float lmt)
{
	colorConverter.Use();
	glUniform1f(colorConverter_lmt, lmt);
}

void __vectorcall __InitShader2()
{
	titleClouds.Load("TitleClouds");
	titleClouds.Use();
	titleClouds_t = titleClouds.UniformLocation("t");
	titleClouds_alpha = titleClouds.UniformLocation("alpha");
	glUniform1i(titleClouds.UniformLocation("clouds"), 0);

	psBlendYSJD.Load("PSBlendYSJD");
	psBlendYSJD.Use();
	glUniform1i(psBlendYSJD.UniformLocation("mainBG"), 0);
	glUniform1i(psBlendYSJD.UniformLocation("blendBG"), 1);

	colorConverter.Load("ColorConverter");
	colorConverter.Use();
	glUniform1f(colorConverter.UniformLocation("tex"), 0);
	colorConverter_lmt = colorConverter.UniformLocation("lmt");
	
}
