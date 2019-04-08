#include "Precompiled.h"
#include "PowerItem.h"
#include "Game.h"
#include "MemoryPool.h"

const uint8_t SE_VOL = 16;

static MemoryPool<Item::PowerItem, 50> p1;
static MemoryPool<Item::BigPowerItem, 20> p2;
static MemoryPool<Item::BluePointItem, 50> p3;
static MemoryPool<Item::HeartKagera, 5> p4;

void Item::PowerItem::OnGot()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	Basic::Sound::SeManager::Play("item00", x, SE_VOL);

	const bool isPowerX2ByDream =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsItemPowerX2() :
		false;

	if (Game::GetGame().GetPlayer().PowerMax())
		Game::GetGame().GetScoreManager().ScoreAdd(isPowerX2ByDream ? 20 : 10);	//额外P点加分
	else
		Game::GetGame().GetPlayer().PowerUp(isPowerX2ByDream ? 0.04 : 0.02);
}

void Item::PowerItem::OnCreate()
{
	SetItemImage(2);
	SetCollisionRadius(8);
}

void Item::PowerItem::Update()
{
	BaseItem::Update();

	const bool isPowerX2ByDream =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsItemPowerX2() :
		false;

	if (m_isPowerX2 != isPowerX2ByDream)
	{
		m_isPowerX2 = isPowerX2ByDream;

		//m_itemImage.SetAlpha(m_isPowerX2 ? 0.5 : 1);
		SetItemImage(m_isPowerX2 ? 6 : 2);
		/*SetFastBufferTarget(
			m_isPowerX2 ?
			Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR :
			Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL
		);*/
		m_drawOnTop = m_isPowerX2;
	}
}

void* Item::PowerItem::operator new(size_t)
{
	return p1.Get();
}

void Item::PowerItem::operator delete(void* ptr)
{
	p1.Return(ptr);
}

void Item::BigPowerItem::OnGot()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	Basic::Sound::SeManager::Play("item00", x, SE_VOL);

	const bool isPowerX2ByDream =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsItemPowerX2() :
		false;

	if (Game::GetGame().GetPlayer().PowerMax())
		Game::GetGame().GetScoreManager().ScoreAdd(25 * isPowerX2ByDream ? 20 : 10);	//额外P点加分
	else
		Game::GetGame().GetPlayer().PowerUp(isPowerX2ByDream ? 0.5 : 0.25);
}

void Item::BigPowerItem::OnCreate()
{
	SetItemImage(1);
	SetCollisionRadius(16);
}

void Item::BigPowerItem::Update()
{
	BaseItem::Update();

	const bool isPowerX2ByDream =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsItemPowerX2() :
		false;

	if (m_isPowerX2 != isPowerX2ByDream)
	{
		m_isPowerX2 = isPowerX2ByDream;

		//m_itemImage.SetAlpha(m_isPowerX2 ? 0.5 : 1);
		SetItemImage(m_isPowerX2 ? 5 : 1);
		/*SetFastBufferTarget(
			m_isPowerX2 ?
			Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR :
			Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL
		);*/
		m_drawOnTop = m_isPowerX2;
	}
}

void* Item::BigPowerItem::operator new(size_t)
{
	return p2.Get();
}

void Item::BigPowerItem::operator delete(void* ptr)
{
	p2.Return(ptr);
}

void Item::BluePointItem::OnGot()
{
	const bool isPowerX2ByDream =
		Game::GetGame().GetPlayer().GetSheild().has_value() ?
		Game::GetGame().GetPlayer().GetSheild()->IsItemPowerX2() :
		false;

	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	Basic::Sound::SeManager::Play("item00", x, SE_VOL);

	unsigned score = 0;
	switch (Game::GetGame().GetLevel()) {
	case StageLevel::E:
		score = 10000;
		break;
	case StageLevel::N:
		score = 14000;
		break;
	case StageLevel::H:
		score = 17000;
		break;
	case StageLevel::L:
		score = 20000;
		break;
	default:
		Log("Item::BigPowerItem::OnGot:Unknown Level");
		break;
	}

	if (isPowerX2ByDream) score *= 2;
	Game::GetGame().GetScoreManager().ScoreAdd(score);
}

void Item::BluePointItem::OnCreate()
{
	SetItemImage(3);
	SetCollisionRadius(8);
}

void* Item::BluePointItem::operator new(size_t)
{
	return p3.Get();
}

void Item::BluePointItem::operator delete(void* ptr)
{
	p3.Return(ptr);
}

void Item::HeartKagera::OnGot()
{
	double x, y;
	Game::GetGame().GetPlayer().GetPos(x, y);
	Basic::Sound::SeManager::Play("item00", x, SE_VOL);
	Game::GetGame().GetScoreManager().AddLifeKagera();
}

void Item::HeartKagera::OnCreate()
{
	SetItemImage(0);
	SetFastBufferTarget(Basic::Video::Renderer2D::FASTBUFFER_BLEND_HDR);
	SetCollisionRadius(16);
	m_drawOnTop = true;
}

void* Item::HeartKagera::operator new(size_t)
{
	return p4.Get();
}

void Item::HeartKagera::operator delete(void* ptr)
{
	p4.Return(ptr);
}