#pragma once
#include "Bullet.h"
#include "Animation2D.h"
#include "Collision.h"

#include "StageData.h"

namespace EnemyBullet {
	class CircleBullet :public Bullet {
	private:
		std::optional<Dmsl::DmslUnit> m_dmslUnit;	//脚本
		Basic::Video::Renderer2D::Animation2D m_bulletImage;	//图像
		bool m_animated;

		int m_minLiveOutScreen;		//在屏幕外依旧生存的时间

		double m_size = 1.0;	//大小比率
		double m_alpha = 1.0;	//透明度

		bool m_dmslUsing;	//是否正在使用Dmsl

		int m_radius;	//子弹图像半径

		bool m_nograze;
		double m_collRadius;	//子弹碰撞半径（原图）

		Collision::Circle m_collision;

		int m_goDie = -1;	//距离完全死亡还有的帧数

		double m_r = 1, m_g = 1, m_b = 1;	//颜色Fliter

		bool m_hdrEnabled = false;
		//double m_hdrSharpness = 0;

		bool m_tailEnabled = false;

		bool m_forcedColli = false;	//无视透明度强制进行碰撞检测

		int m_style, m_subStyle;

		//独立图像角度
		enum ImageAngleMode {
			UseImageAngle,
			UseAngle,
			AutoRotate
		};
		ImageAngleMode m_imageAngleMode;
		double m_imageAngle = 0;

		void addATail(double angle) const;
		void callDmsl();
	public:
		~CircleBullet() override;
		void StopDmsl() override;
		void Update() override;
		void Draw() const override;
		void Kill() override;

		inline void SetImageAngleEnabled(bool b) {
			m_imageAngleMode = b ? UseImageAngle : UseAngle;
		}

		inline void EnableAutoRotate() {
			m_imageAngleMode = AutoRotate;
		}

		inline void SetImageAngle(double angle) {
			m_imageAngle = angle;
		}

		inline void SetForcedColli(bool t)
		{
			m_forcedColli = t;
		}

		//x位置,y位置,风格编号,样式编号,Dmsl程序单元号,最小屏幕外生存时间,是否不可以被大招干掉,Dmsl脚本
		void Create(double x, double y, int style, int subStyle, int dmslUnit, int minLiveOutScreen, bool invalid, const std::shared_ptr<const Dmsl::DmslVarying>& varyingIn);

		//更改弹型
		void ChangeStyle(int style, int subStyle);
		//消耗n1帧，将子弹缩放为n2的大小（倍数，1为原始大小）
		inline void Zoom(int n1, double n2, int slowMotion) {
			m_dataActor.Add(m_size, n2, n1, slowMotion);
			//if (GameObjectWithDmslControl::GetTaskNoteModeEnabled())
				//m_dataActor.Add(m_size, n2, n1, slowMotion);
			//else
				//GameObjectWithDmslControl::AddTask([this, n1, n2, slowMotion]() {
				//m_dataActor.Add(m_size, n2, n1, slowMotion);
			//});
		}

		inline void SetZoom(double zoom) {
			m_size = zoom;
		}

		//消耗n1帧，将子弹调整不透明度为n2（0~1）
		inline void Fade(int n1, double n2, int slowMotion) {
			m_dataActor.Add(m_alpha, n2, n1, slowMotion);
			/*if (GameObjectWithDmslControl::GetTaskNoteModeEnabled())
				m_dataActor.Add(m_alpha, n2, n1, slowMotion);
			else
				GameObjectWithDmslControl::AddTask([this, n1, n2, slowMotion]() {
					m_dataActor.Add(m_alpha, n2, n1, slowMotion);
			});*/
		}

		inline void ColorChange(double r, double g, double b, int slowMotion, int frame) {
			m_dataActor.Add(m_r, r, frame, slowMotion);
			m_dataActor.Add(m_g, g, frame, slowMotion);
			m_dataActor.Add(m_b, b, frame, slowMotion);
		}

		void EnableHDR(bool) override;	//HDR开关
		//void HDRSharpness(double) override;	//HDR亮度
		//void ChangeHDRSharpness(int time, double, int slowMotion) override;	//HDR亮度

		void EnableTail(bool) override;

		bool CollCheckWithCircle(const Collision::Circle&) const override;	//和圆碰撞体进行碰撞检测

		Bullet::Type WhatTypeAmI() const override;

		void* operator new(size_t);
		void operator delete(void* ptr);
	};

	//拖影
	class CircleBulletTail :public Bullet {
	private:
		double m_alpha = 1;
		Basic::Video::Renderer2D::Sprite2D m_image;

		bool m_hdrEnabled = false;
	public:
		void StopDmsl() override;
		void Update() override;
		void Draw() const override;
		void Kill() override;

		void EnableHDR(bool) override;	//HDR开关
		//void HDRSharpness(double) override;	//HDR亮度
		//void ChangeHDRSharpness(int time, double, int slowMotion) override;	//HDR亮度

		void EnableTail(bool) override;

		bool CollCheckWithCircle(const Collision::Circle&) const override;	//和圆碰撞体进行碰撞检测

		Bullet::Type WhatTypeAmI() const override;

		CircleBulletTail(int style, int subStyle, double size, double x, double y, double r, double g, double b, double angle, double alpha, bool hdr);

		void* operator new(size_t);
		void operator delete(void* ptr);
	};
}
