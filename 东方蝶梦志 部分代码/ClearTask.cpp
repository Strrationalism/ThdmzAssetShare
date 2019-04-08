#include "Precompiled.h"
#include "ClearTask.h"
#include "GameObject.h"
#include "Game.h"
#include "Sprite2D.h"
#include "RollingBookPage.h"
#include "FlashObject.h"
class ClearLable :public GameObject {
private:
	Basic::Video::Renderer2D::Sprite2D m_sp;
	double m_alpha = 0;
	Basic::TaskList m_taskList;

public:
	ClearLable() {
		m_died = false;
		m_sp.UseImage("GameUI/StageClear");
		m_sp.SetPos(0, -100);
		m_taskList.AddTask([this]() {
			Game::GetGame().GetDataActor().Add(m_alpha, 1, 100, 1);
		}, 62);
		m_taskList.AddTask([this]() {
			Game::GetGame().GetDataActor().Add(m_alpha, 0, 100, 1);
		}, 461);

		m_taskList.AddTask([this]() {
			auto pScreenshot = Game::GetGame().Screenshot();

			Game::GetGame().AddImportantTask([pScreenshot]()
			{
				Game::GetGame().GetGameActBridge()->OnStageClear();
				Game::GetGame().AddAfterEffect(new AfterEffect::RollingBookPage(181, 4, pScreenshot));
			});

			m_died = true;
		}, 362);

		/*m_taskList.AddTask([this]() {
			m_died = true;

			auto pFlash = new Utils::FlashObject(50, 1, 50, 1, 50);
			pFlash->SetDrawOnTop(true);
			pFlash->SetCall([]() {
				Game::GetGame().GetGameActBridge()->OnStageClear();
			});
			Game::GetGame().GetMiscManagerTop().Insert(pFlash);
		}, 282*2);*/
		m_sp.UpdateVertex();
	}
	void Draw() const override {
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_sp.Draw();
		//glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
	}

	void Update() override {
		m_taskList.Update();
		m_sp.SetAlpha(m_alpha);
	}

	void Kill() override {
		m_died = true;
	}
};

void StageTask::ClearTask::Create(Basic::CSVReader & c, const StageData&)
{
	m_baseScore = c.PopInt();
}

void StageTask::ClearTask::Proc()
{
	if (m_first) {
		m_first = false;
		Game::GetGame().GetTaskList().AddTask([]
		{
			Game::GetGame().GetItemMgr().GetAll();
			if (Game::GetGame().GetPlayer().GetSheild().has_value())
			{
				Game::GetGame().GetPlayer().GetSheild().reset();
				if (Game::GetGame().GetPlayer().GetSheild()->Avliable())
				{
					const double itemX = -Rand() * 320;
					const double itemY = Rand() * 480 - 240;

					if (Game::GetGame().GetStageData()->stageName == "StageEx")
						Game::GetGame().GetItemMgr().AddItem(Item::ItemType::HeartKagera, itemX, itemY, 1);
					Game::GetGame().GetItemMgr().AddItem(Item::ItemType::HeartKagera, itemX, itemY, 1);
				}
			}
			Game::GetGame().GetUILayerGameObjectManager().Insert(new ClearLable);
			Game::GetGame().GetItemMgr().GetAll();
			Basic::Sound::BgmPlayer::Stop(true);

			Game::GetGame().GetPlayer().SetShootBombEnabled(false);
		}, 200);
	}
}

bool StageTask::ClearTask::Finished()
{
	return !m_first;
}