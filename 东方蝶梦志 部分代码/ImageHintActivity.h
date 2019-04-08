#pragma once
#include "Activity.h"
#include "Sprite2D.h"
#include "SecTweenData.h"
#include "SecTaskList.h"

namespace Activities
{
	class ImageHintActivity : public Basic::ActivityEngine::Activity
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_img;
		Basic::SecTweenData<float> m_alpha;
		Basic::SecTaskList m_tl;

		std::function<bool()>* m_func;
	public:
		ImageHintActivity(const std::string& texName, int num,float zoom = 1,std::function<bool()>* func = nullptr);

		void OnEntry() override;
		void OnUpdate(float t) override;
		void OnDraw() const override;
	};
}