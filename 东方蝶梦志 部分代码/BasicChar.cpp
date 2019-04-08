#include "Precompiled.h"
#include "BasicChar.h"
#include "CharMarisa.h"
#include "CharReimu.h"

void Char::LoadAndSetConfig()
{
	Basic::SnowRVReader charData("CharData.rv");

	auto& reimu = Char::Reimu::CharReimu::Get();

	reimu.SetNormalSpeed(charData.GetDouble("REIMU_SPEED"));
	reimu.SetSlowSpeed(charData.GetDouble("REIMU_SLOWSPEED"));
	reimu.InitWithSnowRV(charData);

	auto& marisa = Char::Marisa::CharMarisa::Get();
	marisa.SetNormalSpeed(charData.GetDouble("MARISA_SPEED"));
	marisa.SetSlowSpeed(charData.GetDouble("MARISA_SLOWSPEED"));
	marisa.InitWithSnowRV(charData);
}