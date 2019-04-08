#pragma once
#include "DataActor.h"
#include "TaskList.h"
#include "BaseAfterEffect.h"

namespace AfterEffect {
	class KeepingBarrelBlurWithPlayer :	//跟随玩家的BarrelBlur
		public AfterEffect::BaseAfterEffect
	{
	private:
		Basic::TaskList m_taskList;
		Basic::DataActor m_dataActor;
		double m_power;
		bool m_finished = false;

		double m_offsetX = 0, m_offsetY = 0;
	public:
		//定义效果时间轴，进入时长，保持时长，退出时长，最高Power，进入缓动曲线，退出缓动曲线
		KeepingBarrelBlurWithPlayer(int inFrame, int keepFrame, int endFrame, double highestPower, int fadeInSlowMotion = 0, int fadeOutSlowMotion = 0, double offsetX = 0, double offsetY = 0);

		void Draw() const override;
		void Update() override;

		bool Killed() override;
	};
}