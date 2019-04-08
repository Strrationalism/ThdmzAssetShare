#pragma once
#include "Activity.h"
#include "Sprite2DWStreamLayout.h"

namespace Activities
{
	class LockContryActivity : public Basic::ActivityEngine::Activity
	{
	private:
		Basic::Video::Renderer2D::Sprite2DWStreamLayout m_hint;
	public:
		LockContryActivity();
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnDraw() const override;
		virtual void OnEntry() override;
	};
}