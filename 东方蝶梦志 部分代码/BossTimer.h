#pragma once
#include "GameObject.h"
#include "DataActor.h"
#include "NumberDisplay.h"
namespace Boss {
	class BossTimer :
		public GameObject
	{
	private:
		Basic::Video::Renderer2D::NumberDisplay m_numberDisplay;
		double m_alpha = 0, m_zoom = 1;
		int m_sec = 0;
		bool m_halfAlpha = false;
		Basic::DataActor m_dataActor;
	public:
		BossTimer();
		void Update() override;
		void Draw() const override;

		void Hide();
		void Show();

		void SetTime(int time);

		void Kill() override;
	};
}
