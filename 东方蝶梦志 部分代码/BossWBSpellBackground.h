#pragma once
#include "GameObject.h"

namespace Boss
{
	class BossWBSpellBackground : public GameObject
	{
	private:
		double m_bw = 0;
	public:
		BossWBSpellBackground();
		void Draw() const override;
		void Update() override;

		void SetBW(const double);
		void Kill() override;
	};
}
