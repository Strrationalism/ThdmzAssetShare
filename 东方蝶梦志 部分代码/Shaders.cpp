#include "Shaders.h"
#include "NuzPrecompiled.h"
#include "Shaders.h"
#include "Shaders.h"
#include "GLShader.h"
#include "Window.h"
#include "Globals.h"
#include "glew.h"

static inline void ConvertGamePosToFragShaderPos(float& x, float& y) {
	x = (x + WIDTH / 2) / WIDTH;
	y = (y - HEIGHT / 2) / -HEIGHT;
}

void __InitShader2();

static Basic::Video::LowerRenderer::GLShader
negaCircle, negaCircle2, waterWave, sinClip,
circleWave, barrelBlur, paperRoll, grassAndMoon, marisaMask, godRay, stage3RWall,
flameOnBoard, vistaLight, stage4Grass, stage4Reflaction,
stage6VolumeCloud, stage6VolumeCloudBefore, stageExBW, stageExReflaction, stageExLineDraw,
gaussBlurW, gaussBlurH,colorMixer,dreamSheildBroken,mirrorWBEffect;

static GLuint negaCircle_center, negaCircle_radius;
static GLuint negaCircle2_center, negaCircle2_radius;

static GLuint wave_time, wave_texFactor, wave_ampFactor, wave_center;

static GLuint sinClip_time, sinClip_mul, sinClip_heightMul, sinClip_height;

static GLuint circleWave_time, circleWave_progress;

static GLuint barrelBlur_power, barrelBlur_center;

static GLuint paperRoll_time;

static GLuint grassAndMoon_time;

static GLuint marisaMask_time;

static GLuint godRay_r, godRay_lowesetLum, godRay_center;

static GLuint stage3R_wall_fogEnd, stage3R_wall_time, stage3R_wall_pointLight;

static GLuint flameOnBoard_time;

static GLuint vistaLight_time;

static GLuint stage4Grass_depth;

static GLuint stage4Relfection_time, stageExRelfection_time;

static GLuint gaussBlurW_radius, gaussBlurH_radius;

static GLuint colorMixer_delta;

static GLuint mirrorWBEffect_PlayerPoint;

static GLuint dreamSheildBroken_center, dreamSheildBroken_time, dreamSheildBroken_mode;
void Basic::Shaders::Init()
{
	int screenW, screenH;
	SDL_GL_GetDrawableSize(Basic::Video::Window::GetWindow(), &screenW, &screenH);

	negaCircle.Load("NegaCircle", "NegaCircle");
	negaCircle.Use();
	glUniform1i(negaCircle.UniformLocation("texture"), 0);
	negaCircle_center = negaCircle.UniformLocation("circleCenter");
	negaCircle_radius = negaCircle.UniformLocation("circleRadius2");

	negaCircle2.Load("NegaCircle2", "NegaCircle");
	negaCircle2.Use();
	glUniform1i(negaCircle2.UniformLocation("texture"), 0);
	negaCircle2_center = negaCircle2.UniformLocation("circleCenter");
	negaCircle2_radius = negaCircle2.UniformLocation("circleRadius2");

	waterWave.Load("Wave");
	waterWave.Use();
	glUniform1i(waterWave.UniformLocation("tex"), 0);
	wave_time = waterWave.UniformLocation("time");
	wave_texFactor = waterWave.UniformLocation("texFactor");
	wave_ampFactor = waterWave.UniformLocation("ampFactor");
	wave_center = waterWave.UniformLocation("center");

	sinClip.Load("SinClip");
	sinClip.Use();
	glUniform1i(sinClip.UniformLocation("texture"), 0);
	sinClip_mul = sinClip.UniformLocation("mul");
	sinClip_time = sinClip.UniformLocation("time");
	sinClip_heightMul = sinClip.UniformLocation("heightMul");
	sinClip_height = sinClip.UniformLocation("height");

	circleWave.Load("CircleWave");
	circleWave.Use();
	glUniform1i(circleWave.UniformLocation("tex"), 0);
	circleWave_time = circleWave.UniformLocation("time");
	circleWave_progress = circleWave.UniformLocation("progress");

	barrelBlur.Load("BarrelBlur");
	barrelBlur.Use();
	glUniform1i(barrelBlur.UniformLocation("tex"), 0);
	barrelBlur_power = barrelBlur.UniformLocation("barrelPower");
	barrelBlur_center = barrelBlur.UniformLocation("center");

	paperRoll.Load("PaperRoll");
	paperRoll.Use();
	glUniform1i(paperRoll.UniformLocation("sourceTex"), 1);
	glUniform1i(paperRoll.UniformLocation("targetTex"), 0);
	paperRoll_time = paperRoll.UniformLocation("time");

	grassAndMoon.Load("GrassAndMoon");
	grassAndMoon.Use();
	grassAndMoon_time = grassAndMoon.UniformLocation("iGlobalTime");
	glUniform2f(grassAndMoon.UniformLocation("iResolution"), (float)screenW, (float)screenH);
	glUniform1i(grassAndMoon.UniformLocation("iChannel0"), 0);

	marisaMask.Load("MarisaMask");
	marisaMask.Use();
	glUniform1i(marisaMask.UniformLocation("tex"), 0);
	glUniform1i(marisaMask.UniformLocation("mask"), 1);
	marisaMask_time = marisaMask.UniformLocation("time");

	godRay.Load("GodRay");
	godRay.Use();
	godRay_lowesetLum = godRay.UniformLocation("lowestLum");
	godRay_r = godRay.UniformLocation("GlowRange");
	godRay_center = godRay.UniformLocation("centerpos");
	glUniform1i(godRay.UniformLocation("CC_Texture0"), 0);

	stage3RWall.Load("Stage3RWall", "Stage3RWall");
	stage3RWall.Use();
	stage3R_wall_fogEnd = stage3RWall.UniformLocation("fogEnd");
	stage3R_wall_time = stage3RWall.UniformLocation("time");
	stage3R_wall_pointLight = stage3RWall.UniformLocation("pointLight");
	glUniform1i(stage3RWall.UniformLocation("tex"), 0);

	flameOnBoard.Load("FlameOnBoard");
	flameOnBoard.Use();
	flameOnBoard_time = flameOnBoard.UniformLocation("Time");
	glUniform1i(flameOnBoard.UniformLocation("Cloud"), 0);

	vistaLight.Load("VistaLight");
	vistaLight.Use();
	vistaLight_time = vistaLight.UniformLocation("time");
	glUniform1i(vistaLight.UniformLocation("cloud"), 0);
	

	stage4Grass.Load("Stage4Grass");
	stage4Grass.Use();
	glUniform1i(stage4Grass.UniformLocation("screenShot"), 0);
	glUniform1i(stage4Grass.UniformLocation("grassImage"), 1);
	stage4Grass_depth = stage4Grass.UniformLocation("depth");

	stage4Reflaction.Load("Stage4Reflaction", "Stage4Water");
	stage4Reflaction.Use();
	glUniform1i(stage4Reflaction.UniformLocation("tex"), 0);
	glUniform1i(stage4Reflaction.UniformLocation("waterImage"), 1);
	stage4Relfection_time = stage4Reflaction.UniformLocation("time");

	stage6VolumeCloud.Load("Stage6VolumeCloud");
	stage6VolumeCloud.Use();
	glUniform1i(stage4Reflaction.UniformLocation("FB"), 0);

	stage6VolumeCloudBefore.Load("Stage6VolumeCloudBefore");
	stage6VolumeCloudBefore.Use();
	glUniform1i(stage4Reflaction.UniformLocation("FB"), 0);

	stageExReflaction.Load("StageExReflaction", "Stage4Water");
	stageExReflaction.Use();
	glUniform1i(stageExReflaction.UniformLocation("tex"), 0);
	glUniform1i(stageExReflaction.UniformLocation("waterImage"), 1);
	stageExRelfection_time = stageExReflaction.UniformLocation("time");

	stageExBW.Load("StageExBW");
	stageExBW.Use();
	glUniform1i(stageExBW.UniformLocation("tex"), 0);

	stageExLineDraw.Load("LineDraw");
	stageExLineDraw.Use();
	glUniform1i(stageExLineDraw.UniformLocation("iChannel0"),0);

	gaussBlurW.Load("GaussBlurW");
	gaussBlurW.Use();
	glUniform1i(gaussBlurW.UniformLocation("fb"), 0);
	glUniform2f(gaussBlurW.UniformLocation("resolution"), (float)screenW, (float)screenH);
	gaussBlurW_radius = gaussBlurW.UniformLocation("radius");
	
	gaussBlurH.Load("GaussBlurH");
	gaussBlurH.Use();
	glUniform1i(gaussBlurH.UniformLocation("fb"), 0);
	glUniform2f(gaussBlurH.UniformLocation("resolution"), (float)screenW, (float)screenH);
	gaussBlurH_radius = gaussBlurH.UniformLocation("radius");

	colorMixer.Load("ColorMixer");
	colorMixer.Use();
	glUniform1i(colorMixer.UniformLocation("tex"), 0);
	colorMixer_delta = colorMixer.UniformLocation("delta");

	dreamSheildBroken.Load("DreamSheildBroken");
	dreamSheildBroken.Use();
	glUniform1i(dreamSheildBroken.UniformLocation("fbo"), 0);
	dreamSheildBroken_center = dreamSheildBroken.UniformLocation("center");
	dreamSheildBroken_time = dreamSheildBroken.UniformLocation("time");
	dreamSheildBroken_mode = dreamSheildBroken.UniformLocation("mode");

	mirrorWBEffect.Load("MirrorWBEffect");
	mirrorWBEffect.Use();
	glUniform1i(mirrorWBEffect.UniformLocation("Frame"), 0);
	mirrorWBEffect_PlayerPoint = mirrorWBEffect.UniformLocation("PlayerPoint");

	
	__InitShader2();

	glUseProgram(0);
	auto relaseShaderCompilerAddress = SDL_GL_GetProcAddress("glReleaseShaderCompiler");
	if (relaseShaderCompilerAddress) {
		auto releaseShaderCompiler = static_cast<void(*)()> (relaseShaderCompilerAddress);
		releaseShaderCompiler();
		Log("Basic::Shaders::Init Shader Compiler Released.");
	}
}


void Basic::Shaders::NegaCircle(float centerX, float centerY, float size)
{
	negaCircle.Use();
	glUniform2f(negaCircle_center, centerX, centerY);
	glUniform1f(negaCircle_radius, (size));
}

void Basic::Shaders::NegaCircle2(float centerX, float centerY, float size)
{
	negaCircle2.Use();
	glUniform2f(negaCircle2_center, centerX, centerY);
	glUniform1f(negaCircle2_radius, (size));
}

void Basic::Shaders::WaterWave(float time, float texFactor, float ampFactor, float centerX, float centerY)
{
	waterWave.Use();
	glUniform1f(wave_time, time);
	glUniform1f(wave_ampFactor, ampFactor);
	glUniform1f(wave_texFactor, texFactor);

	//坐标转换
	centerY = -centerY;
	centerX += WIDTH / 2;
	centerY += HEIGHT / 2;
	centerX /= WIDTH;
	centerY /= HEIGHT;
	glUniform2f(wave_center, centerX, centerY);
}

void Basic::Shaders::SinClip(float time, float mul, float heightMul, float height)
{
	sinClip.Use();
	glUniform1f(sinClip_mul, mul);
	glUniform1f(sinClip_time, time);
	glUniform1f(sinClip_heightMul, heightMul);
	glUniform1f(sinClip_height, height);
}

void Basic::Shaders::CircleWave(float time, float progress)
{
	circleWave.Use();
	glUniform1f(circleWave_time, time);
	glUniform1f(circleWave_progress, progress);
}

void Basic::Shaders::BarrelBlur(float power, float centerX, float centerY)
{
	barrelBlur.Use();
	glUniform1f(barrelBlur_power, power * 5);
	ConvertGamePosToFragShaderPos(centerX, centerY);
	glUniform2f(barrelBlur_center, centerX, centerY);
}

void Basic::Shaders::PaperRoll(float time)
{
	paperRoll.Use();
	glUniform1f(paperRoll_time, time);
}



void Basic::Shaders::GrassAndMoon(float time)
{
	grassAndMoon.Use();
	glUniform1f(grassAndMoon_time, time);
}

void Basic::Shaders::MarisaMask(float time)
{
	marisaMask.Use();
	glUniform1f(marisaMask_time, time);
}

void Basic::Shaders::GodRay(float r, float lowestLum, float x, float y)
{
	godRay.Use();
	glUniform1f(godRay_r, r);
	glUniform1f(godRay_lowesetLum, lowestLum);

	//坐标转换
	y = -y;
	x += WIDTH / 2;
	y += HEIGHT / 2;
	x /= WIDTH;
	y /= HEIGHT;
	glUniform2f(godRay_center, x, y);
}

#include "Game.h"

void Basic::Shaders::Stage3RWall(float fogEnd,float time, float playerX, float playerY)
{
	stage3RWall.Use();
	glUniform1f(stage3R_wall_fogEnd, fogEnd);
	glUniform1f(stage3R_wall_time, time);
	glUniform2f(stage3R_wall_pointLight, playerX, playerY);
}

void Basic::Shaders::FlameOnBoard(float time)
{
	flameOnBoard.Use();
	glUniform1f(flameOnBoard_time, time);
}

void Basic::Shaders::VistaLight(float time)
{
	vistaLight.Use();
	glUniform1f(vistaLight_time, time);
}

void Basic::Shaders::Stage4Grass(float depth)
{
	stage4Grass.Use();
	glUniform1f(stage4Grass_depth, depth);
}

void Basic::Shaders::Stage4Relfection(float time)
{
	stage4Reflaction.Use();
	glUniform1f(stage4Relfection_time, time);
}

void Basic::Shaders::Stage6VolumeCloudBefore()
{
	stage6VolumeCloudBefore.Use();
}

void Basic::Shaders::Stage6VolumeCloud()
{
	stage6VolumeCloud.Use();
}

void Basic::Shaders::StageExBW()
{
	stageExBW.Use();
}

void Basic::Shaders::StageExRelfection(float time)
{
	stageExReflaction.Use();
	glUniform1f(stageExRelfection_time, time);
}

void Basic::Shaders::StageExLineDraw()
{
	stageExLineDraw.Use();
}

void Basic::Shaders::GaussBlurW(float radius)
{
	gaussBlurW.Use();
	glUniform1f(gaussBlurW_radius, radius);
}

void Basic::Shaders::GaussBlurH(float radius)
{
	gaussBlurH.Use();
	glUniform1f(gaussBlurH_radius, radius);
}

void Basic::Shaders::ColorMixer(float delta)
{
	colorMixer.Use();
	glUniform1f(colorMixer_delta, delta);
}

void Basic::Shaders::DreamSheildBroken(float centerX, float centerY, float time,int mode)
{
	dreamSheildBroken.Use();
	glUniform2f(dreamSheildBroken_center, (centerX+640) / 720, 1.0F - (centerY+360) / 720);
	glUniform1f(dreamSheildBroken_time, time);
	glUniform1i(dreamSheildBroken_mode, mode);
}

void Basic::Shaders::MirrorModeWBEffect(float centerX, float centerY)
{
	mirrorWBEffect.Use();
	centerX += WIDTH / 2;
	centerY += HEIGHT / 2;
	centerX /= WIDTH;
	centerY /= HEIGHT;
	glUniform2f(mirrorWBEffect_PlayerPoint, centerX, 1.0f-centerY);
}

