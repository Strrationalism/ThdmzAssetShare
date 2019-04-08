#pragma once
#include "GameObject.h"
#include "Sprite2D.h"
#include "DataActor.h"
#include "GLTexture.h"
#include <memory>
namespace Boss {
	class BossMahoujin :
		public GameObject
	{
	private:
		Basic::Video::LowerRenderer::GLTexture m_circleWaveFB;
		Basic::Video::Renderer2D::Sprite2D m_sp, m_shadow;
		double m_x = 0, m_y = 0;
		double m_angle = 0;
		double m_progress = 0;
		double m_size = 1.0f;
		double m_cnt = 0;
		double m_alpha = 1;
		int m_frames = 0;
		bool m_effectSinSize = false;
		Basic::DataActor m_dataActor;
	public:
		BossMahoujin();

		inline void SetPos(double x, double y) {
			m_sp.SetPos(x, y);
			m_shadow.SetPos(x, y);
			m_x = x;
			m_y = y;
		}

		void Hide();
		void Show();

		void Update() override;
		void Draw() const override;
		void Kill() override;

		void EffectSinSize(bool b) {
			m_effectSinSize = b;
		}
	};
}
