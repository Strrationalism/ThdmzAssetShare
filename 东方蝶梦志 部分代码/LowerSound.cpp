#include "NuzPrecompiled.h"
#include "Globals.h"
#include "Log.h"
#include "LowerSound.h"
#include "BGMPackFormat.h"
#include "BGMPlayer.h"
using namespace std;



#pragma region SEMixer
void _InitSE();
void _Mixer_SE(Uint8* output, int len);
#pragma endregion

#pragma region BGMMixer
void _InitBGM();
void _Mixer_BGM(Uint8* output, int len);
#pragma endregion


#pragma region MainMixer
static void __cdecl _Mixer(void*, Uint8* output, int len) {
	if (len > Basic::Sound::LowerSound::MAX_BYTES) Crash(L"音频设备不兼容！");

	
	memset(output, 0, len);

	_Mixer_BGM(output, len);
	_Mixer_SE(output, len);
}
#pragma endregion

#pragma region DeviceManager

void Basic::Sound::LowerSound::InitSound()
{
	SDL_AudioSpec fmt;
	fmt.freq = 44100;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 2048;
	fmt.callback = _Mixer;
	fmt.userdata = NULL;

	//SDL_BuildAudioCVT(&_cvt, AUDIO_U8, 1, 22050, fmt.format, fmt.channels, fmt.freq);

	if (SDL_OpenAudio(&fmt, NULL) >= 0) {
		SDL_LockAudio();
		_InitSE();
		_InitBGM();
		SDL_UnlockAudio();
	}
	else {
		Log("Warning:无法打开音频。");
	}
	SDL_PauseAudio(0);
}



void Basic::Sound::LowerSound::QuitSound()
{
	SDL_PauseAudio(1);

	SDL_AudioQuit();
}
#pragma endregion


