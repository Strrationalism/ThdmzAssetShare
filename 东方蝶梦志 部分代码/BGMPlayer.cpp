#include "NuzPrecompiled.h"
#include "BGMPlayer.h"
#include "CSVReader.h"
#include "Asset.h"
#include "LowerSound.h"
#include "Globals.h"

const Basic::Sound::BGMData::BGMData* Basic::Sound::BgmPlayer::m_now = nullptr;
int Basic::Sound::BgmPlayer::m_nowBeatNum = 0;
bool Basic::Sound::BgmPlayer::m_isBeat = false;
bool Basic::Sound::BgmPlayer::m_paused = true;
double Basic::Sound::BgmPlayer::m_cnt = 0;

void Basic::Sound::BgmPlayer::Play(int n,bool fadeIn)
{
	m_now = &BGMData::GetBGMData(n);
	m_nowBeatNum = -1;
	m_cnt = 0;
	LowerSound::PlayBGM(*m_now, fadeIn);
	
	m_paused = false;
}

void Basic::Sound::BgmPlayer::Esc()
{
	//Mix_PauseMusic();
	m_paused = true;
}

void Basic::Sound::BgmPlayer::Resume()
{
	//Mix_ResumeMusic();
	m_paused = false;
}

void Basic::Sound::BgmPlayer::Stop(bool fadeOut)
{
	LowerSound::StopBGM(fadeOut);

	//停止状态下使用510号BGM的节奏
	m_now = &BGMData::GetBGMData(510);
	m_nowBeatNum = -1;
	m_cnt = 0;
	m_paused = false;
}

static std::map<int, Basic::Sound::BGMData::BGMData> bgmData;
void Basic::Sound::BGMData::Init()
{
	Basic::CSVReader csv;
	csv.Load("BGMInfo.csv");

	while (!csv.IsLastLine()) {
		int first = csv.PopInt();
		auto& b = bgmData[first];
		b.title = first;
		b.frame_beat = LPS/(csv.PopDouble()/60);
		b.beatType = csv.PopInt();
		b.headNum = csv.PopInt();
		b.loopNum = csv.PopInt();

		csv.NextLine();
	}

	Basic::Sound::BgmPlayer::Stop(false);
}

const Basic::Sound::BGMData::BGMData& Basic::Sound::BGMData::GetBGMData(int n)
{
	return AT(bgmData,n);
}
