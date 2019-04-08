#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "DataActor.h"
//这个效果比较特殊
//需要拖影效果

namespace Item
{
	class DreamPointItem :
		public GameObject
	{
	private:
		double m_x, m_y;
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		Basic::Video::Renderer2D::Sprite2D m_sp;

		bool m_part = false;	//粒子系统

		const bool m_isDarkTheme;

		DreamPointItem(double x, double y, double dreamPower, int wait = 0);
	public:
		static void AddToGame(double x, double y, double dreamPower,int wait = 0);

		void Update() override;
		void Draw() const override;
		void Kill() override;

		void* operator new(size_t);
		void operator delete(void*);
	};
}