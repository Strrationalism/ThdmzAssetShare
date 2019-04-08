#pragma once
#include "BaseAfterEffect.h"
#include "DataActor.h"
namespace AfterEffect {
	class DreamSheildBrokenEffect :
		public AfterEffect::BaseAfterEffect
	{
	public:
		enum class Mode : int
		{
			FullScreen = 0,
			HalfScreen = 1
		};
	private:
		Basic::DataActor m_dataActor;
		const float m_x, m_y;
		double m_time = 0;
		const Mode m_mode;
	public:

		DreamSheildBrokenEffect(float x,float y, Mode mode);
		void Draw() const override;
		void Update() override;
		bool Killed() override;
	};
}