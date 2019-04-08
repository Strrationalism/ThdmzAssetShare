#pragma once
#include "BasicInput.h"
#include <map>

#include <ostream>
#include <istream>
#include <string>
#include <vector>
namespace Inputer {
	class Movie
	{
	private:
		struct Frame {
			std::vector<Basic::Input::Message> msg;
			uint8_t beat;
		};
		std::map<uint32_t, Frame> m_frames;

		std::optional<uint32_t> m_seed;
	public:
		//录制时
		void PushMsg(const Basic::Input::Message&);
		void PushBeat(int);

		//播放时
		const Basic::Input::Message& GetEvent(int frame, int num) const;
		unsigned EventNum(int frame) const;
		int GetBeat(int frame) const;

		//存储
		void SaveToStream(std::ostream& out) const;
		void LoadFromStream(std::istream& in);

		void Clear();

		unsigned GetSeed();
	};
}
