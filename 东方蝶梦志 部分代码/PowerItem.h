#pragma once
#include "BaseItem.h"

namespace Item {
	class PowerItem :public BaseItem
	{
	private:
		bool m_isPowerX2 = false;
	public:
		void OnGot() override;
		void OnCreate() override;

		void Update() override;

		void* operator new(size_t);
		void operator delete(void*);
	};

	class BigPowerItem :public BaseItem {
	private:
		bool m_isPowerX2 = false;
	public:
		void OnGot() override;
		void OnCreate() override;

		void Update() override;

		void* operator new(size_t);
		void operator delete(void*);
	};

	class BluePointItem :public BaseItem {
	public:
		void OnGot() override;
		void OnCreate() override;

		void* operator new(size_t);
		void operator delete(void*);
	};

	class HeartKagera :public BaseItem {
	public:
		void OnGot() override;
		void OnCreate() override;

		void* operator new(size_t);
		void operator delete(void*);
	};
}