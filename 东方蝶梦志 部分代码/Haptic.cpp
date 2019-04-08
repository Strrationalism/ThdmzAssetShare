#include "NuzPrecompiled.h"
#include "Haptic.h"
#include "CSSReader.h"
#include "Log.h"
static SDL_Haptic* haptic = nullptr;
static std::map<std::string, int> effects;

static void deleteHaptic()
{
	if (haptic) {
		SDL_HapticClose(haptic);
	}
	haptic = nullptr;
}

static float haptic_strength = 1.0;	//手柄震动强度
void __SetHapticStrength(float s)
{
	haptic_strength = s;
}

float __GetHapticStregth()
{
	return haptic_strength;
}

void Basic::Haptic::SetHaptic(void * v)
{
	deleteHaptic();
	haptic = (SDL_Haptic*)v;
	if (v == nullptr) return;
	if (SDL_HapticRumbleSupported(haptic) == 0) {
		deleteHaptic();
		Log("Gamepad haptic not supposed.");
	}
	else SDL_HapticRumbleInit(haptic);
}


void Basic::Haptic::Play(int time)
{
	if(haptic && haptic_strength > 0) SDL_HapticRumblePlay(haptic, haptic_strength, time);
	Log("Haptic:%d", time);
	
}
