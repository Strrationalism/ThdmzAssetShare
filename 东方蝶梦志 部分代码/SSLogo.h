#pragma once
#include "Activity.h"
#include "Sprite2D.h"
#include "SecTaskList.h"
#include "SecTweenData.h"
#include "TaskList.h"
#include <any>
namespace Activities::Title
{
	class SSLogo : public Basic::ActivityEngine::Activity
	{
	private:
		Basic::Video::Renderer2D::Sprite2D m_sslogo;
		Basic::SecTweenData<double> m_alpha = 0;
		Basic::SecTaskList m_taskList;
		Basic::TaskList m_frameTaskList;

		std::any& m_textureCache;

		bool m_useShader = false;

		std::function<void()> m_runGame;
		int initGameSystem();

	public:
		SSLogo(std::any& texCache,const std::function<void()>& runGame);
		void OnEntry() override;
		void OnDraw() const override;
		void OnUpdate(float) override;

	};
}