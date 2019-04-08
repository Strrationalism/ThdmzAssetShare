#pragma once
#include <vector>
#include <memory>
#include "MenuItem.h"

namespace Menus
{
	template <typename TContent>
	class WRollingPage : public Basic::Menu::MenuItem
	{
	private:
		std::vector<std::unique_ptr<TContent>> m_content;

	public:

		WRollingPage(std::queue<std::unique_ptr<TContent>>& content, int num)
		{
			for (int i = 0; i < num; ++i)
			{
				if (content.empty())
					break;

				auto con = std::move(content.front());
				content.pop();

				con->SetPageLine(i);
				m_content.push_back(std::move(con));
			}
		}

		static std::vector<std::shared_ptr<Basic::Menu::MenuItem>>
			MakePages(std::queue<std::unique_ptr<TContent>>& content, int num)
		{
			std::vector<std::shared_ptr<Basic::Menu::MenuItem>> ret;
			while (!content.empty())
			{
				ret.push_back(make_shared<WRollingPage<TContent>>(content, num));
			}

			return ret;
		}

		void OnSelectChanged(int offset) override
		{
			for (auto& p : m_content)
				p->Visible(offset == 0);
		}

		void OnInitSelect(int offset) override
		{
			OnSelectChanged(offset);
		}

		void Update(float deltaTime) override
		{
			for (auto& p : m_content)
				p->Update(deltaTime);
		}

		void Draw() const override
		{
			for (auto& p : m_content)
				p->Draw();
		}

		auto begin()
		{
			return m_content.begin();
		}

		auto end()
		{
			return m_content.end();
		}
	};
}