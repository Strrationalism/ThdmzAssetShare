#include "Precompiled.h"
#include "ItemManager.h"
#include <algorithm>
#include "PowerItem.h"
#include "Game.h"

void Item::ItemManager::AddItem(ItemType it, double x, double y, int num, double boom)
{
	for (int i = 0; i < num; ++i) {
		int rand1 = (int)(Game::GetGame().GetInputer().Rand2() * RAND_MAX);
		int rand2 = (int)(Game::GetGame().GetInputer().Rand2() * RAND_MAX);

		Game::GetGame().GetTaskList().AddTask([=]() {
			BaseItem* item = nullptr;
			switch (it) {
			case ItemType::Power:
				item = new PowerItem;
				break;
			case ItemType::BigPower:
				item = new BigPowerItem;
				break;
			case ItemType::BluePoint:
				item = new BluePointItem;
				break;
			case ItemType::HeartKagera:
				item = new HeartKagera;
				break;
			}
			item->Create(x, y, (-boom / 4 + boom / 8 * (double)(rand1 % num) / num) * 2, (boom / 4 + boom / 4 * (double)(rand2 % num) / num) / 16);
			m_items.push_back(std::unique_ptr<BaseItem>(item));
		}, 0);
	}
}

void Item::ItemManager::GetAll()
{
	for (auto& p : m_items)
		p->Get();
}

void Item::ItemManager::Draw() const
{
	static std::vector<BaseItem*> onTop;

	for (auto& p : m_items) {
		if (p->DrawOnTop())
			onTop.push_back(&(*p));
		else
			p->Draw();
	}

	for (auto p : onTop)
		p->Draw();
	onTop.clear();
}

void Item::ItemManager::Update()
{
	int size = (int)m_items.size();
	//#pragma omp parallel for
	for (int i = 0; i < size; ++i) {
		AT(m_items, i)->Update();
	}

	m_items.erase(
		std::remove_if(m_items.begin(), m_items.end(), [](const auto& i) {
		return i->Died();
	}
	), m_items.end());
}