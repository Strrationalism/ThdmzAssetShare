#include "NuzPrecompiled.h"
#include "LockContryActivity.h"
#include "BGMPlayer.h"
#include "Globals.h"

Activities::LockContryActivity::LockContryActivity():
	m_hint
	{
		SDL_Rect{ -640,0,1280,48 },
		Basic::Video::Renderer2D::Sprite2DWStreamLayout::LayoutMode::CenterTop
	}
{
	
}

void Activities::LockContryActivity::OnUpdate(float deltaTime)
{
}

void Activities::LockContryActivity::OnDraw() const
{
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	m_hint.Draw();
}

void Activities::LockContryActivity::OnEntry()
{
	Basic::Sound::BgmPlayer::Play(524,true);

	m_hint.SetSpriteSizeOffset(15, 0);
	GetCtxFont1().SetSprite2DWStreamToString(L"本地区不可用", m_hint);
	

	for (auto& sp : m_hint)
	{
		sp->SetZoom(1);
	}

	m_hint.UpdateStream();
}
