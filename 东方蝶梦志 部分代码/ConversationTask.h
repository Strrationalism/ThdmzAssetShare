#pragma once
#include "StageTask.h"
#include "Conversation.h"
#include "ConversationLogic.h"
#include "TextWindow.h"
#include "ImageCacheManager.h"
#include <queue>
namespace StageTask {
	class Conversation :
		public StageTask::BaseTask
	{
	private:
		ConversationScript::Script m_scriptLoader;

		::Conversation::ConversationLogic m_logic;
		::Conversation::ConversationLogicDisplay* m_display = nullptr;

		GameObject* m_left, *m_right;

		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_leftLHCache, m_rightLHCache;

		bool m_logicRunning = false;

		GameObject* m_textWindow = nullptr;

		bool m_clickEnabled;

		bool m_first = true;
		bool m_end = false;
	public:
		void Create(Basic::CSVReader&, const StageData&) override;	//从C列开始读取参数
		void Proc() override;	//执行任务
		void Over();
		bool Finished() override;	//是否已完成任务,一定会执行一次。
	};
}