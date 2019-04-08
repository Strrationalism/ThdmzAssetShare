#include "Precompiled.h"
#include "BaseInputer.h"
#include <ctime>
#include "BGMPlayer.h"
using namespace Basic::Input;

bool Inputer::NormalInputer::PopMessage(Message & msg)
{
	if (m_msgs.empty()) return false;
	else {
		msg = m_msgs.front();
		m_msgs.pop();
		return true;
	}
}

int Inputer::NormalInputer::GetFrameBeat()
{
	if (Basic::Sound::BgmPlayer::IsBeatFrame()) {
		return Basic::Sound::BgmPlayer::GetBeatNum();
	}
	else return -1;
}

void Inputer::NormalInputer::PushMessage(const Message & msg)
{
	m_msgs.push(msg);
}

unsigned Inputer::NormalInputer::GetRandSeed()
{
	return time(nullptr);
}