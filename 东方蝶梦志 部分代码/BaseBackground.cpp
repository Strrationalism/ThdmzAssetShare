#include "Precompiled.h"
#include "BaseBackground.h"

#include "BackgroundStage1.h"
#include "BackgroundStage2.h"
#include "BackgroundStage3.h"
#include "BackgroundStage4.h"
#include "BackgroundStage5.h"
#include "BackgroundStage6.h"
#include "BackgroundStageEx.h"

void Background::BaseBackground::UpdateWithoutGame()
{
	this->Update();
}

std::unique_ptr<Background::BaseBackground> Background::BaseBackground::CreateBackground(int n)
{
	Background::BaseBackground* p = nullptr;
	switch (n) {
	case 1:
		p = new BackgroundStage1;
		break;
	case 2:
		p = new BackgroundStage2;
		break;
	case 3:
		p = new BackgroundStage3;
		break;
	case 4:
		p = new BackgroundStage4;
		break;
	case 5:
		p = new BackgroundStage5;
		break;
	case 6:
		p = new BackgroundStage6;
		break;
	case 7:
		p = new BackgroundStageEx;
		break;
	default:
		std::terminate();
		break;
	}
	return std::unique_ptr<BaseBackground>(p);
}

void Background::BaseBackground::SetUsingBeautifulBG(bool b)
{
	m_usingBeautifulBG = b;
}

bool Background::BaseBackground::GetUsingBeautifulBG()
{
	return m_usingBeautifulBG;
}

bool Background::BaseBackground::m_usingBeautifulBG;