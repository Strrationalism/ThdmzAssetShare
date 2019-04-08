#pragma once
#include <string>

namespace Basic {
	namespace Sound {
		class SeManager final
		{
		public:
			static void Init();
			static void Quit();

			static void Play(const std::string& name, int16_t pan, uint8_t volume = 21);
			static void PlayHigh(const std::string& name, int16_t pan,uint8_t volume = 21);
			static void Update();
		};

	}
}