#pragma once
#include "BaseAfterEffect.h"
#include "DataActor.h"
#include "glew.h"

namespace AfterEffect {
	class DreamWaterWave :
		public AfterEffect::BaseAfterEffect
	{
	private:
		Basic::DataActor m_dataActor;
		double m_cnt = 0;
		double m_r, m_g, m_b, m_a;
		double m_cntX, m_cntY;
		GLenum m_blendMode = GL_ONE_MINUS_SRC_ALPHA, m_blendModeSrc = GL_SRC_ALPHA;
	public:
		DreamWaterWave(double x = 0, double y = HEIGHT / 2);
		void Draw() const override;
		inline void SetBlendMode(GLenum src, GLenum dst) {
			m_blendMode = dst;
			m_blendModeSrc = src;
		}
		inline void SetColor(double r, double g, double b, double alpha, double er, double eg, double eb, double ea) {
			m_r = r;
			m_g = g;
			m_b = b;
			m_a = alpha;

			m_dataActor.Add(m_r, er, 100, 1);
			m_dataActor.Add(m_g, eg, 100, 1);
			m_dataActor.Add(m_b, eb, 100, 1);
			m_dataActor.Add(m_a, ea, 100, 1);
		}
		void Update() override;
		bool Killed() override;
	};
}