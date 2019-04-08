#include "NuzPrecompiled.h"
#include "LowerSound.h"
#include "Globals.h"


const int MAX_SE = 128;	//最大同时播放的音效个数

using namespace std;

static struct {
	bool live = false;
	Uint32 nowPos = 0;
	Uint8* buf;
	Uint32 size;
	Uint8 volume;
	float pan;
}_playingSE[MAX_SE];	//正在播放的SE列表

						//SE转换器
static SDL_AudioCVT _cvt;


static std::vector<Sint16> _seMixBuf;

void _Mixer_SE(Uint8* output, int len)
{
	//混合音效
	for (auto& se : _playingSE) {
		if (se.live) {
			if (se.nowPos + len < se.size) {
				_seMixBuf.resize(len / 2);
				memcpy(&_seMixBuf[0],  se.buf + se.nowPos, _seMixBuf.size() * 2);

				se.nowPos += len;
			}
			else {
				_seMixBuf.resize((se.size - se.nowPos) / 2);
				memcpy(&_seMixBuf[0], se.buf + se.nowPos, _seMixBuf.size() * 2);

				se.live = false;
			}

			//此代码被优化到下边
			/*bool leftChannel = true;
			for (Sint16& smp : _seMixBuf) {
				
				float gain = se.pan;
				if (leftChannel && gain > 0) {
					smp *= 1 - gain;
				}

				else if (!leftChannel && gain < 0) {
					smp *= 1 + gain;
				}

				leftChannel = !leftChannel;
			}*/

			//左声道
			float gain = se.pan;
			for (size_t i = 0; i < _seMixBuf.size(); i += 2)
			{
				if (gain > 0)
					_seMixBuf[i] *= 1 - gain;
			}

			//右声道
			for (size_t i = 1; i < _seMixBuf.size(); i += 2)
			{
				if (gain < 0)
					_seMixBuf[i] *= 1 + gain;
			}

			SDL_MixAudio(output, (Uint8*)&_seMixBuf[0], _seMixBuf.size() * 2, se.volume / 0.618);
		}
	}
}

void _InitSE() {
	for (auto p : _playingSE) {
		p.live = false;
	}

	_seMixBuf.reserve(65536);
}

void Basic::Sound::LowerSound::PlaySound(uint8_t* buf, uint32_t len, uint8_t volume, int16_t pan)
{
	SDL_LockAudio();
	for (auto& se : _playingSE) {
		if (!se.live) {
			se.live = true;
			se.buf = (uint8_t*)buf;
			se.nowPos = 0;
			se.size = len;
			se.volume = volume;
			se.pan = Clamp(-1, 1, pan / 640.0 * 0.618);
			break;
		}
	}
	SDL_UnlockAudio();
}
