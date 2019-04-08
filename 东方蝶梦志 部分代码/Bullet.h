#pragma once
#include "Dmsl.h"
#include "GameObjectWithDmslControl.h"
#include <memory>
#include "StageData.h"
#include "Collision.h"
namespace EnemyBullet {
	class Bullet :public GameObjectWithDmslControl {
	protected:
		//bool m_invailed;	//是否可以被大招消灭掉
		static Dmsl::GameObjectLibraryLinkTable m_dmslLinkTable;
		static std::unique_ptr<Dmsl::DmslVirtualMachine> m_bulletScript;

		bool m_invincible = false;

		static void linkScript(Dmsl::DmslVirtualMachine&);
	public:
		enum class Type {
			Circle, CircleTail, LimitedLazer, UnlimitedLazer, Grid
		};

		~Bullet() override;

		static void SetLevel(StageLevel l);
		static void LoadScript();

		inline bool GetInvincible() {
			return m_invincible;
		}

		inline void SetInvincible(bool b) {
			m_invincible = b;
		}

		void Update() override = 0;
		void Draw() const override = 0;
		void Kill() override = 0;
		virtual void StopDmsl() = 0;
		virtual Type WhatTypeAmI() const = 0;

		virtual bool CollCheckWithCircle(const Collision::Circle&) const = 0;	//和圆碰撞体进行碰撞检测

		virtual void EnableHDR(bool) = 0;	//HDR开关
		//virtual void HDRSharpness(double) = 0;	//HDR亮度
		//virtual void ChangeHDRSharpness(int time,double, int slowMotion) = 0;	//HDR亮度

		virtual void EnableTail(bool) = 0;	//启动或关闭拖影

		//x位置,y位置,风格编号,样式编号,Dmsl程序单元号,最小屏幕外生存时间,是否不可以被大招干掉,Dmsl脚本
		//废弃？
		//virtual void Create(double x, double y, int style, int subStyle, int dmslUnit, int minLiveOutScreen, bool invalid, Dmsl::DmslVirtualMachine& dmsl, double dmslArg) = 0;
	};
}
