#include "StaffActivity.h"
#include "NuzPrecompiled.h"
#include "StaffActivity.h"
#include "SnowRVReader.h"
#include "CSVReader.h"
#include "BGMPlayer.h"
#include "ActivityEngine.h"
#include "WrappedSaveRepActivity.h"
#include "TitleMainMenu.h"
#include "GameRebuilder.h"
#include "CharReimu.h"
using namespace std;

void Activities::StaffActivity::changeToBackground(int bkID,bool skipToBossMode)
{
	m_bkAlpha.Tween(0, 2, 1);
	m_tl.AddTask([this,bkID, skipToBossMode] {
		m_bk = Background::BaseBackground::CreateBackground(bkID);
		m_bk->Init();
		m_bk->Reset();
		if (skipToBossMode)
			m_bk->SkipToBossMode();
		m_bkAlpha.Tween(1, 2, 1);
	}, 3);
}

Activities::StaffActivity::StaffActivity(std::unique_ptr<MovieData::MovieData>&& movieData):
	m_movieData { move(movieData) }
{
	m_alpha = 0;

	Basic::CSVReader csv;
	csv.Load("StaffImages.csv");
	
	while (!csv.IsLastLine())
	{
		const auto height = csv.PopDouble();
		const auto zoom = csv.PopDouble();
		const auto tex = csv.PopString();
		const auto spNum = csv.PopInt();
		
		ImageUnit iu;
		iu.yOffset = m_height;
		m_height += height;

		iu.sprite = make_unique<Basic::Video::Renderer2D::Sprite2D>();
		iu.sprite->UseImage(tex, spNum);
		iu.sprite->SetZoom(zoom);

		m_staffList.push_back(move(iu));

		csv.NextLine();
	}

	m_bk = Background::BaseBackground::CreateBackground(1);
	m_bk->Init();
	m_bk->Reset();
}

void Activities::StaffActivity::OnEntry()
{
	m_ySub = 0;


	m_tl.AddTask(
		[ this] {
			m_alpha.Tween(1.0, 9);
		}, 10
	);

	m_tl.AddTask([this] {
		m_ySub.Tween(m_height - 600, 157.1f, 0);
	}, 22.7f);

	m_tl.AddTask([this] {
		Basic::Sound::BgmPlayer::Stop(false);

		m_tl.AddTask([this] {
			if (m_movieData)
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(make_unique<WrappedSaveRepActivity>(move(m_movieData)));
			else
				Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Goto(make_unique<Activities::Title::TitleMainMenu>());
		}, 5);
	},190.5f);

	m_bkAlpha = 0;

	m_tl.AddTask([this] {
		m_bkAlpha.Tween(1, 2, 1);
	}, 45.38f - 1.5f);

	m_tl.AddTask([this] {
		changeToBackground(2);
	}, 66.87f-3.5f);

	m_tl.AddTask([this] {
		changeToBackground(3);
	}, 60+28.29f-3.5f);

	m_tl.AddTask([this] {
		changeToBackground(4);
	}, 60+50.4f-3.5f);

	m_tl.AddTask([this] {
		changeToBackground(5,true);
	}, 120+14.46f-3.5f);

	m_tl.AddTask([this] {
		changeToBackground(6,true);
	}, 120+38.71f-3.5f);

	m_tl.AddTask([this] {
		m_bkAlpha.Tween(0, 6.04f - 4.37f, 0);
	}, 180 + 4.37f);

	m_tl.AddTask([this] {
		m_alpha.Tween(0, 10.16f-8.62f, 0);
	}, 180 + 8.62f);

	Basic::Sound::BgmPlayer::Play(526);
}

void Activities::StaffActivity::OnUpdate(float t)
{
	m_tl.Update(t);
	m_ySub.Update(t);
	m_alpha.Update(t);
	m_bkAlpha.Update(t);

	for (auto& p : m_staffList)
	{
		p.sprite->SetAlpha(m_alpha);
		p.sprite->SetPos(0, p.yOffset - m_ySub);
		p.sprite->UpdateVertex();
	}

	m_timer++;
	if (m_timer % 2 == 0)
	{
		m_bk->UpdateWithoutGame();
	}
}

void Activities::StaffActivity::OnDraw() const
{
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);

	m_bk->Draw();

	const auto darkness = 1 - m_bkAlpha;
	if (darkness > 0)
	{
		glColor4f(0, 0, 0, darkness);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		{
			glVertex2f(-640, -360);
			glVertex2f(640, -360);
			glVertex2f(640, 360);
			glVertex2f(-640, 360);
		}
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}

	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
	for (const auto& p : m_staffList)
		p.sprite->DrawToFastBuffer();
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();
	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
}
