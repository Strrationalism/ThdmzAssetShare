#pragma once
#include "MenuItem.h"
#include <vector>
#include <memory>

namespace Basic::Menu
{
	class Menu
	{
	private:
		std::vector<std::shared_ptr<MenuItem>> m_items;
		std::function<void()> m_onUp, m_onDown;
		int m_select;

		bool m_enabled = false;

		bool m_clamped = false;

	public:
		Menu(std::vector<std::shared_ptr<MenuItem>>&& items,int select = 0);
		void Draw() const;
		void Update(float deltaTime);

		inline void SetClamp(bool b)
		{
			m_clamped = b;
		}

		inline int GetSelected() const
		{
			return m_select;
		}

		void Up();
		void Down();
		void SelectItem(int index);
		void Enter();
		void RefreshSelectIndex();

		inline void SetOnUp(const decltype(m_onUp)& up)
		{
			m_onUp = up;
		}

		inline void SetOnDown(const decltype(m_onDown)& dn)
		{
			m_onDown = dn;
		}

		inline void SetEnabled(bool b)
		{
			m_enabled = b;
		}

		inline bool Enabled() const
		{
			return m_enabled;
		}

		inline auto Size() const
		{
			return m_items.size();
		}

		inline auto begin()
		{
			return m_items.begin();
		}

		inline auto end()
		{
			return m_items.end();
		}

		template<typename TMenuItem,typename TFor>
		inline void For(TFor&& f)
		{
			for (const auto& p : m_items)
			{
				try
				{
					f(dynamic_cast<TMenuItem&>(*p));
				}
				catch (std::bad_cast)
				{

				}
			}
		}
	};
}