#pragma once
#include <cstdint>
#include <future>
namespace Basic {
	namespace Sound{
		namespace BGMData {
			struct BGMData;
		}
		namespace LowerSound {

			const int MAX_BYTES = 8192;

			void InitSound();
			std::future<void> CacheBGM();
			void QuitSound();

			void PlaySound(uint8_t* buf, uint32_t len,uint8_t volume,int16_t pan);
			void PlayBGM(const Basic::Sound::BGMData::BGMData& bgmData,bool fadeIn = false);
			void StopBGM(bool fadeOut = true);
			void PauseBGM(bool fadeOut = true);
			void ResumeBGM(bool fadeOut = true);

			void PushBGMStack();
			void PopBGMStack();

			/* “Ù∆µ∏Ò Ω£∫
			 * 44100Hz S16
			 */
		}
	}
}