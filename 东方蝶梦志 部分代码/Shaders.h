#pragma once
#include "glew.h"
namespace Basic {

	namespace Shaders {
		void Init();

		//贴图纹理在0号纹理单元
		
		//反色圈准备
		void NegaCircle(float centerX, float centerY, float size);
		void NegaCircle2(float centerX, float centerY, float size);

		//水波
		void WaterWave(float time, float texFactor = 12.0, float ampFactor = 0.01,float centerX = 0,float centerY = 0);

		//高度Sin形剪裁
		void SinClip(float time, float mul,float heightMul,float height);

		//圆形大小限定的水波
		void CircleWave(float time,float progress);

		//Barrel Blur效果
		void BarrelBlur(float power = 0.3f,float centerX = 0,float centerY = 0);

		//书页卷动效果
		//TEXTURE1 - Source
		//TEXTURE0 - Target
		void PaperRoll(float time);

		//艹和月亮
		void GrassAndMoon(float time);

		//Mask
		//0 - Texture
		//1 - Mask
		void MarisaMask(float time);

		//丁达尔光照图
		void GodRay(float r, float lowestLum, float x, float y);

		//Stage3 墙壁
		void Stage3RWall(float fogEnd,float time, float playerX, float playerY);

		//地板上的火焰
		//	需要在0上绑定Cloud
		void FlameOnBoard(float time);

		//极光
		//	需要0上绑定Cloud
		void VistaLight(float time);

		void Stage4Grass(float depth);
		void Stage4Relfection(float time);

		//6面极光
		//	需要在0上绑定FB
		void Stage6VolumeCloudBefore();
		void Stage6VolumeCloud();

		//Ex面黑白效果
		//	在0上绑定被黑白化的图像
		void StageExBW();
		void StageExRelfection(float time);

		//Ex面描边效果
		//	在0上绑定要描边的图像
		void StageExLineDraw();

		//高斯模糊
		void GaussBlurW(float radius);
		void GaussBlurH(float radius);

		//颜色混合器
		void ColorMixer(float delta);

		//破盾特效
		void DreamSheildBroken(float centerX, float centerY, float time,int mode);

		//Mirror模式下的颜色特效
		void MirrorModeWBEffect(float centerX, float centerY);
	}
}
