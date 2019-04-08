#pragma once
#include <memory>
#include <vector>
#include "BaseItem.h"
namespace Item {
	enum class ItemType {
		Power,
		BigPower,
		BluePoint,
		HeartKagera
	};

	class ItemManager
	{
	private:
		std::vector<std::unique_ptr<BaseItem> > m_items;
		bool m_itemSystemEnabled = true;
	public:
		void AddItem(ItemType, double x, double y, int num = 1, double boom = 15);
		void GetAll();

		void Draw() const;
		void Update();

		inline void Clear() {
			m_items.clear();
		}

		inline void SetEnabled(bool b) {
			m_itemSystemEnabled = b;
		}

		inline bool GetEnabled() {
			return m_itemSystemEnabled;
		}
	};
}