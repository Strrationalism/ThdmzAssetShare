#include "Precompiled.h"
#include "NonFullHeart.h"
#include "Shaders.h"
#include "Game.h"
#include "Log.h"
#include "BGMPlayer.h"

UI::NonFullHeart::NonFullHeart()
{
	m_alpha = 1;
	m_data = 0;
	m_time = 0;
	m_cup.UseImage("GameUI/SmallUI", 3);
	m_water.UseImage("GameUI/HeartWater");

	m_cup.SetPos(-595, -285);
	m_water.SetPos(-595, -285);

	m_timeHeartBeat = 0;
}

void UI::NonFullHeart::Draw() const
{
	m_cup.Draw();
	Basic::Shaders::SinClip(m_time, 2.4f, 0.05f, (1 - m_data));
	m_water.Draw();
	glUseProgram(0);
}

void UI::NonFullHeart::Update()
{
	m_time += 0.1;
	m_water.SetAlpha(m_alpha);
	m_cup.SetAlpha(m_alpha);

	m_timeHeartBeat += 0.03;
	if (Basic::Sound::BgmPlayer::IsBeatFrame()) {
		switch (Basic::Sound::BgmPlayer::GetBgmData().beatType) {
		case 4:	//四四拍
			if (Basic::Sound::BgmPlayer::GetBeatNum() == 1 || Basic::Sound::BgmPlayer::GetBeatNum() == 3) {
				m_timeHeartBeat = 0.05;
			}
			break;
		case 3:	//四三拍
			if (Basic::Sound::BgmPlayer::GetBeatNum() == 2) {
				m_timeHeartBeat = 0.05;
			}
			break;
		}
	}

	float ss = pow(m_timeHeartBeat, .2)*0.5 + 0.5;
	ss = 1.0 + ss*0.5*sin(m_timeHeartBeat*6.2831*3.0)*exp(-m_timeHeartBeat*4.0)*0.8;
	m_water.SetZoom(ss);
	m_cup.SetZoom(ss);
	m_cup.UpdateVertex();
	m_water.UpdateVertex();
}

void UI::NonFullHeart::HalfAlpha()
{
	Game::GetGame().GetDataActor().Add(m_alpha, 0.4, 40, 2);
}

void UI::NonFullHeart::FullAlpha()
{
	Game::GetGame().GetDataActor().Add(m_alpha, 1, 40, 2);
}

void UI::NonFullHeart::SetData(double height)
{
	Game::GetGame().GetDataActor().Add(m_data, height, 40, 2);
}

void UI::NonFullHeart::SetDataUseDefaultHeight(int kagera)
{
	switch (kagera) {
	case 0:
		SetData(0);
		break;
	case 1:
		SetData(0.37);
		break;
	case 2:
		SetData(0.42);
		break;
	case 3:
		SetData(0.51);
		break;
	case 4:
		SetData(0.59);
		break;
	case 5:
		SetData(0.67);
		break;
	case 6:
		SetData(1);
		break;
	default:
		Log("Warning:NonFullHeart::SetDataUseDefaultHeight::Invaild Argument %d", kagera);
		break;
	}
}