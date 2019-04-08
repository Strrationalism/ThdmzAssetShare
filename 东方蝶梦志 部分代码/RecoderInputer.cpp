#include "Precompiled.h"
#include "RecoderInputer.h"
#include "BGMPlayer.h"
#include <ctime>
using namespace Basic::Input;
inline static bool canPushToMovie(const Basic::Input::Message & m) {
	if (m.type == Basic::Input::Message::Type::VecMotion) return true;

	if (m.type == Basic::Input::Message::Type::KeyDown ||
		m.type == Basic::Input::Message::Type::KeyUp) {
		if (m.key == Basic::Input::Key::Slow ||
			m.key == Basic::Input::Key::Bomb ||
			m.key == Basic::Input::Key::Shoot)
			return true;
	}

	return false;
}

bool Inputer::RecoderInputer::PopMessage(Message & msg)
{
	if (m_msgs.empty()) return false;
	else {
		msg = m_msgs.front();
		m_msgs.pop();
		return true;
	}
}

void Inputer::RecoderInputer::PushMessage(const Message & m)
{
	m_msgs.push(m);

	if (canPushToMovie(m)) m_movie.PushMsg(m);
}

int Inputer::RecoderInputer::GetFrameBeat()
{
	int beat;
	if (Basic::Sound::BgmPlayer::IsBeatFrame()) {
		beat = Basic::Sound::BgmPlayer::GetBeatNum();
	}
	else beat = -1;
	m_movie.PushBeat(beat);
	return beat;
}

unsigned Inputer::RecoderInputer::GetRandSeed()
{
	return m_movie.GetSeed();
}