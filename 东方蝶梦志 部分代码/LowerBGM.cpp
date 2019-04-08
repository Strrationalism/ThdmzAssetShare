#include "NuzPrecompiled.h"
#include "LowerSound.h"
#include "BGMPackFormat.h"
#include "BGMPlayer.h"
#include <random>
#include <cassert>
#include <optional>

#pragma region PackState
static Basic::Sound::LowerSound::BGMPackFormat::Header packHeader;
static std::ifstream pack;
static std::vector<Uint8> loadedPack;
#pragma endregion


#pragma region TypeDefine

struct BGMInfo
{
	Uint32 key;
	Basic::Sound::LowerSound::BGMPackFormat::Header::Chunk head, loop;
};

struct PlayingState
{
	enum State
	{
		PLAYING,
		PAUSED,
		STOPPED,
		STOPPING_FADE,
		PAUSING_FADE,
		RESUMEING_FADE
	}state;

	Uint32 pos;

	float fadevolume;

	bool playingHead;
	BGMInfo bgm;
};

#pragma endregion

#pragma region States

static std::stack<PlayingState> stateStack;

static bool bgmEnabled = true;
#pragma endregion


#pragma region Mixer
void _Mixer_BGM(Uint8* output, int len)
{
	if (!bgmEnabled) return;

	auto& state = stateStack.top();

	Uint8 bgmCache[Basic::Sound::LowerSound::MAX_BYTES] = { 0 };

	if (state.state == state.RESUMEING_FADE)
	{
		state.fadevolume += 0.01F;
		if (state.fadevolume >= 1)
		{
			state.state = state.PLAYING;
			state.fadevolume = 1;
		}
	}
	
	if (
		state.state == state.PLAYING ||
		state.state == state.PAUSING_FADE ||
		state.state == state.STOPPING_FADE ||
		state.state == state.RESUMEING_FADE
		)
	{
		const auto& playingChunk = state.playingHead ? state.bgm.head : state.bgm.loop;
		const auto seekPos = playingChunk.offset + state.pos;
		const auto readSize = min(len,static_cast<int>(playingChunk.size - state.pos));
		//pack.seekg(seekPos, std::ios::beg);	//优化

		if (pack.good())
			pack.read(reinterpret_cast<char*>(bgmCache), readSize);
		else
			memcpy(bgmCache, &loadedPack[0] + seekPos, readSize);
			//std::copy(&loadedPack[0] + seekPos, &loadedPack[0] + seekPos + readSize, bgmCache);


		state.pos += readSize;

		//回归循环部头部进行声音补全
		if (readSize < len)
		{
			const auto& loopChunk = state.bgm.loop;

			//设置循环部状态机
			state.playingHead = false;
			state.pos = 0;
			
			const auto completionSize = len - readSize;
			if (!(completionSize + state.pos < loopChunk.size))
			{
				state.state = state.STOPPED;
				return;
			}

			if (pack.good())
			{
				pack.seekg(loopChunk.offset, std::ios::beg);
				pack.read(reinterpret_cast<char*>(&bgmCache[readSize]), completionSize);
			}
			else
			{
				//std::copy(, &loadedPack[0] + loopChunk.offset + completionSize,);
				memcpy(bgmCache + readSize, &loadedPack[0] + loopChunk.offset, completionSize);
			}
			state.pos += completionSize;
		}
	}

	if (state.state == state.STOPPING_FADE)
	{
		state.fadevolume *= 0.9f;
		
		if (state.fadevolume <= 0.01f)
		switch (state.state)
		{
		case PlayingState::STOPPING_FADE:
			state.state = PlayingState::STOPPED;
			break;
		default:
			assert(false);
			break;
		}
	}

	SDL_MixAudio(output, bgmCache, len, static_cast<int>(0.6f * SDL_MIX_MAXVOLUME * state.fadevolume));
}
#pragma endregion

#pragma region Init
void _InitBGM()
{
	//Open Pack and Read Pack Header
	pack.open("thbgm.dat", std::ios::binary);
	if (!pack.good())
	{
		pack.open("../thbgm.dat", std::ios::binary);
	}

	pack.read(reinterpret_cast<char*>(&packHeader), sizeof(packHeader));

	//Bottom Playing State
	PlayingState bottom;
	bottom.state = bottom.STOPPED;
	bottom.pos = 0;
	bottom.fadevolume = 1.f;
	bottom.playingHead = false;
	bottom.bgm.key = 0;
	bottom.bgm.head.offset = 0;
	bottom.bgm.head.size = 0;
	bottom.bgm.loop = bottom.bgm.head;

	stateStack.push(bottom);

}
#pragma endregion

#pragma region Debug
void __dbg__DisableBGM()
{
	SDL_LockAudio();
	bgmEnabled = false;
	SDL_UnlockAudio();
}
#pragma endregion



void Basic::Sound::LowerSound::PlayBGM(const Basic::Sound::BGMData::BGMData& bgmData,bool fadeIn)
{
	if (!bgmEnabled) return;
	SDL_LockAudio();

	const auto& header = packHeader;

	//覆盖状态机
	auto& st = stateStack.top();

	if (fadeIn)
	{
		st.state = st.RESUMEING_FADE;
		st.fadevolume = 0.f;
	}
	else {
		st.state = st.PLAYING;
		st.fadevolume = 1.f;
	}
	st.pos = 0;
	
	st.playingHead = bgmData.headNum != -1;

	st.bgm.key = bgmData.beatType + 10 * bgmData.title;
	st.bgm.head.offset = 0;
	st.bgm.head.size = 0;
	if (st.playingHead)
	{
		st.bgm.head = header.index[bgmData.headNum];
	}
	st.bgm.loop = header.index[bgmData.loopNum];

	//优化Mixer后产生的Seek
	const auto& playingChunk = st.playingHead ? st.bgm.head : st.bgm.loop;
	const auto seekPos = playingChunk.offset;

	if(pack.good())
		pack.seekg(seekPos, std::ios::beg);

	SDL_UnlockAudio();
}



void Basic::Sound::LowerSound::StopBGM(bool fadeOut)
{
	if (!bgmEnabled) return;
	SDL_LockAudio();
	if (fadeOut)
	{
		stateStack.top().state = PlayingState::STOPPING_FADE;
	}
	else
	{
		stateStack.top().state = PlayingState::STOPPED;
	}
	SDL_UnlockAudio();
}

void Basic::Sound::LowerSound::PauseBGM(bool fadeOut)
{
	if (!bgmEnabled) return;
	SDL_LockAudio();
	if (fadeOut)
	{
		assert(false);
	}
	else
	{
		stateStack.top().state = PlayingState::PAUSED;
	}
	SDL_UnlockAudio();
}

std::future<void> Basic::Sound::LowerSound::CacheBGM()
{
	return std::async([] {
		//SDL_PauseAudio(1);
		SDL_LockAudio();

		pack.seekg(0, std::ios::end);
		const auto packSize = pack.tellg();

		pack.seekg(0, std::ios::beg);
		loadedPack.resize(packSize);
		pack.read((char*)loadedPack.data(), packSize);

		pack.close();

		SDL_UnlockAudio();
		//SDL_PauseAudio(0);
	});
}

void Basic::Sound::LowerSound::ResumeBGM(bool fadeOut)
{
	if (!bgmEnabled) return;
	SDL_LockAudio();
	if (fadeOut)
	{
		assert(false);
	}
	else
	{
		stateStack.top().state = PlayingState::PLAYING;
	}
	SDL_UnlockAudio();
}

void Basic::Sound::LowerSound::PushBGMStack()
{
	if (!bgmEnabled) return;
	SDL_LockAudio();
	PlayingState s;
	s.fadevolume = 0;
	s.playingHead = 0;
	s.pos = 0;
	s.state = PlayingState::STOPPED;
	stateStack.push(s);
	SDL_UnlockAudio();
}

void Basic::Sound::LowerSound::PopBGMStack()
{
	if (!bgmEnabled) return;
	SDL_LockAudio();
	stateStack.pop();

	const auto& state = stateStack.top();
	const auto& playingChunk = state.playingHead ? state.bgm.head : state.bgm.loop;
	const auto seekPos = playingChunk.offset + state.pos;

	if(pack.good())
		pack.seekg(seekPos, std::ios::beg);

	SDL_UnlockAudio();
}
