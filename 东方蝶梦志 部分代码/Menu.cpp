#include "NuzPrecompiled.h"
#include "Menu.h"

Basic::Menu::Menu::Menu(std::vector<std::shared_ptr<MenuItem>>&& items,int sel):
	m_items{ std::move(items) },m_select{sel}
{
	for (std::size_t i = 0; i < m_items.size(); ++i)
		m_items.at(i)->OnInitSelect(m_select - i);
}

void Basic::Menu::Menu::Draw() const
{
	static std::stack<MenuItem*> drawTop;

	for (const auto& item : m_items)
	{
		if (item->IsTopDraw())
			drawTop.push(item.get());
		else
			item->Draw();
	}

	while (!drawTop.empty())
	{
		drawTop.top()->Draw();
		drawTop.pop();
	}
}

void Basic::Menu::Menu::Update(float deltaTime)
{
	for (const auto& item : m_items)
		item->Update(deltaTime);
}

void Basic::Menu::Menu::Up()
{
	if (m_enabled)
	{
		if (m_clamped)
		{
			if (m_select < int(m_items.size() - 1))
			{
				m_select++;
				for (std::size_t i = 0; i < m_items.size(); ++i)
					m_items.at(i)->OnSelectChanged(m_select - i);

				m_onUp();
			}
		}
		else
		{
			m_select++;
			m_select %= m_items.size();
			for (std::size_t i = 0; i < m_items.size(); ++i)
				m_items.at(i)->OnSelectChanged(m_select - i);

			m_onUp();
		}
	}
}

void Basic::Menu::Menu::Down()
{
	if (m_enabled)
	{
		if (m_clamped)
		{
			if (m_select > 0)
			{
				m_select--;
				for (std::size_t i = 0; i < m_items.size(); ++i)
					m_items.at(i)->OnSelectChanged(m_select - i);

				m_onDown();
			}
		}
		else
		{
			m_select--;
			m_select += m_items.size();
			m_select %= m_items.size();
			for (std::size_t i = 0; i < m_items.size(); ++i)
				m_items.at(i)->OnSelectChanged(m_select - i);

			m_onDown();
		}
	}
}

void Basic::Menu::Menu::SelectItem(int index)
{
	m_select = index % m_items.size();
	for (std::size_t i = 0; i < m_items.size(); ++i)
		m_items.at(i)->OnSelectChanged(m_select - i);

	m_onDown();
}

void Basic::Menu::Menu::Enter()
{
	if (m_enabled)
	{
		for (std::size_t i = 0; i < m_items.size(); ++i)
			m_items.at(i)->OnEntry(m_select - i);
	}
}

void Basic::Menu::Menu::RefreshSelectIndex()
{
	for (std::size_t i = 0; i < m_items.size(); ++i)
		m_items.at(i)->OnSelectChanged(m_select - i);
}
