#include "Precompiled.h"
#include "ReplayerInputer.h"
#include "Game.h"

using namespace Basic::Input;

void Inputer::ReplayerInputer::clearQueues()
{
	while (!m_msg.empty()) {
		m_msg.pop();
	}
}

void Inputer::ReplayerInputer::loadFrameQueue()
{
	auto frame = Game::GetGame().GetGameTime();

	auto size = m_movie.EventNum(frame);
	for (unsigned i = 0; i < size; ++i)
		m_msg.push(m_movie.GetEvent(frame, i));
}

void Inputer::ReplayerInputer::prep()
{
	if (Game::GetGame().GetGameTime() != m_nowFrame) {
		clearQueues();
		loadFrameQueue();
		m_nowFrame = Game::GetGame().GetGameTime();
	}
}

bool Inputer::ReplayerInputer::PopMessage(Message & msg)
{
	prep();
	if (m_msg.empty()) return false;
	else {
		msg = m_msg.front();
		m_msg.pop();
		return true;
	}
}

void Inputer::ReplayerInputer::PushMessage(const Message &)
{
}

int Inputer::ReplayerInputer::GetFrameBeat()
{
	return m_movie.GetBeat(Game::GetGame().GetGameTime());
}

unsigned Inputer::ReplayerInputer::GetRandSeed()
{
	return m_movie.GetSeed();
}

void Inputer::ReplayerInputer::Reset()
{
	m_nowFrame = -1;
	clearQueues();
}