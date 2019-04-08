#pragma once
#include "GameObject.h"
#include "DataActor.h"
#include "TaskList.h"
#include <functional>
namespace Utils {
	/* 闪屏特效 */
	class FlashObject :
		public GameObject
	{
	private:
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		double m_r = 1, m_g = 1, m_b = 1, m_alpha = 0;

		std::function<void()> m_call = []() {};
	public:
		//必须在此处设置时间轴
		FlashObject(int leftTime, int leftSlowMotion, int rightTime, int rightSlowMotion, int keep, double highestAlpha = 1);

		//设置颜色
		inline void SetColor(double r, double g, double b) {
			m_r = r;
			m_g = g;
			m_b = b;
		}

		//闪屏中间时要处理的事情
		inline void SetCall(std::function<void()> c) {
			m_call = c;
		}

		//设置DrawOnTop
		void SetDrawOnTop(bool b) {
			m_drawOnTop = b;
		}

		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}