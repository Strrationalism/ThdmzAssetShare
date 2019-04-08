#include "Precompiled.h"
#include "Movie.h"
#include "Game.h"

using namespace Basic::Input;

void Inputer::Movie::PushMsg(const Basic::Input::Message & m)
{
	m_frames[Game::GetGame().GetGameTime()].msg.push_back(m);
}

void Inputer::Movie::PushBeat(int i)
{
	m_frames[Game::GetGame().GetGameTime()].beat = i;
}

const  Basic::Input::Message&  Inputer::Movie::GetEvent(int frame, int num) const
{
	return m_frames.at(frame).msg.at(num);
}

unsigned Inputer::Movie::EventNum(int frame) const
{
	if (m_frames.count(frame))
		return m_frames.at(frame).msg.size();
	else
		return 0;
}

int Inputer::Movie::GetBeat(int frame) const
{
	return m_frames.count(frame) ? m_frames.at(frame).beat : -1;
}

void Inputer::Movie::SaveToStream(std::ostream& out) const
{
	//写出版本号
	//out.write((const char*)&VERSION, sizeof(VERSION));

	//写出帧数
	uint32_t size = m_frames.size();
	out.write((char*)&size, sizeof(size));

	//写出每一帧
	for (auto& p : m_frames) {
		out.write((char*)&p.first, sizeof(p.first));	//写出帧号

		//写出Msg
		size = p.second.msg.size();
		out.write((char*)&size, sizeof(size));

		for (auto& msg : p.second.msg)
			out.write((char*)&msg, sizeof(msg));

		//写出每一帧的节拍
		out.write((char*)&p.second.beat, sizeof(p.second.beat));
	}

	if (!m_seed.has_value())
		Crash(L"这个影片没有随机数种子!");
	//写出种子
	out.write((char*)&(*m_seed), sizeof(*m_seed));
}

void Inputer::Movie::LoadFromStream(std::istream & in)
{
	Clear();

	//读入版本号
	//unsigned inVer;
	//in.read((char*)&inVer, sizeof(VERSION));

	//if (inVer != VERSION)
		//Crash(L"录像读取失败！");

	//读入帧数
	uint32_t size;
	uint32_t numFrames;
	in.read((char*)&numFrames, sizeof(numFrames));

	for (uint32_t i = 0; i < numFrames; ++i) {
		//读入帧号
		uint32_t frame;
		in.read((char*)&frame, sizeof(i));

		auto& frameObj = m_frames[frame];

		//读入Msg
		in.read((char*)&size, sizeof(size));
		for (unsigned j = 0; j < size; ++j) {
			Message m;
			in.read((char*)&m, sizeof(m));
			frameObj.msg.push_back(m);
		}

		//读入每一帧的节拍
		in.read((char*)&frameObj.beat, sizeof(frameObj.beat));
	}

	//读入种子
	uint32_t seed;
	in.read((char*)&seed, sizeof(seed));
	m_seed = seed;
}

void Inputer::Movie::Clear()
{
	m_frames.clear();
}


unsigned Inputer::Movie::GetSeed()
{
	if (!m_seed.has_value())
		m_seed.emplace(time(NULL));
	return *m_seed;
}