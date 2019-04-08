#pragma once
#include <queue>
#include "StageData.h"
#include "BasicChar.h"
#include "TitleMenuStackFrame.h"
#include "GameActivity.h"
#include <memory>
namespace Activities::Title::MenuStack
{
	class TitleToGame : public TitleMenuStackFrame
	{
	private:
		std::shared_ptr<Basic::ActivityEngine::Activity> m_gact;
		Basic::SecTweenData<float> m_rollTimer;
		Basic::SecTaskList m_tl;
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_game,m_menu;
	public:
		TitleToGame(const std::shared_ptr<Basic::ActivityEngine::Activity>&);

		void OnEntryToThis() override;
		void OnDraw() const override;				//普通渲染时
		void OnDrawYSJD() const override;			//颜色减淡渲染时
		void OnDrawPost() const override;			//后处理渲染时

		void OnUpdate(float deltaTime) override;	//每帧
		void OnEvent(const Basic::Input::Message& msg) override;
	};
}
