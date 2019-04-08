#include "NuzPrecompiled.h"
#include "TitleJoystickConfig.h"
#include "SeManager.h"
#include "Haptic.h"

class JSMenuItem : public Basic::Menu::MenuItem
{
public:
	virtual void Hide() = 0;
	virtual void Show() = 0;
};

class JSButtonSetting : public JSMenuItem
{
private:
	Basic::Video::Renderer2D::Sprite2D m_sp;
	bool m_selected = false;

	Basic::SecTweenData<float> m_spAlpha = 0;
	Basic::SecTweenData<float> m_hiderAlpha = 1;

	std::function<void()> m_func;
public:
	JSButtonSetting(std::function<void()>&& func)
	{
		m_func = std::move(func);
	}

	void OnSelectChanged(int offset) override
	{
		m_selected = offset == 0;

		if (m_selected)
			m_spAlpha.Tween(0.85f, 0.25f, 1);
		else
			m_spAlpha.Tween(0.65f, 0.25f, 1);

		m_sp.UseImage("GamePadConfig", 2);
		m_sp.SetZoom(0.5f);
		m_sp.SetPos(-150, -50);
		m_sp.SetAlpha(0);
		m_sp.UpdateVertex();
	}

	void OnInitSelect(int offset) override
	{
		OnSelectChanged(offset);
	}
	
	void Update(float deltaTime) override
	{
		m_hiderAlpha.Update(deltaTime);
		m_spAlpha.Update(deltaTime);
		m_sp.SetAlpha(m_spAlpha * m_hiderAlpha);
	}

	void Draw() const override
	{
		m_sp.Draw();
	}

	void Hide() override
	{
		m_hiderAlpha.Tween(0, 0.25f, 1);
	}

	void Show() override
	{
		m_hiderAlpha.Tween(1, 0.25f, 1);
	}

	void OnEntry(int offset) override
	{
		if(offset == 0)
			m_func();
	}
};

extern float __GetHapticStregth();
extern void __SetHapticStrength(float s);

class JSHapticSetting : public JSMenuItem
{
private:
	Basic::Video::Renderer2D::Sprite2D m_sp,m_force;
	bool m_selected = false;

	Basic::SecTweenData<float> m_spAlpha = 0, m_hiderAlpha = 1;

	int m_forceLevel;
	static constexpr float ForceLevel[] = { 0,0.33f,0.66f,1.0f };
public:
	void Hide() override
	{
		m_hiderAlpha.Tween(0, 0.25f, 1);
	}

	void Show() override
	{
		m_hiderAlpha.Tween(1, 0.25f, 1);
	}

	JSHapticSetting()
	{
		const auto level = __GetHapticStregth();

		if (level <= 0)
			m_forceLevel = 0;
		else if (level > 0 && level <= 0.33f)
			m_forceLevel = 1;
		else if (level > 0.33f && level <= 0.66f)
			m_forceLevel = 2;
		else
			m_forceLevel = 3;
	}

	void OnSelectChanged(int offset) override
	{
		m_selected = offset == 0;

		if (m_selected)
			m_spAlpha.Tween(0.85f, 0.25f, 1);
		else
			m_spAlpha.Tween(0.6f, 0.25f, 1);

		m_sp.UseImage("GamePadConfig", 3);
		m_sp.SetZoom(0.5f);
		m_sp.SetPos(-150, 50);
		m_sp.SetAlpha(0);
		m_sp.UpdateVertex();

		m_force.UseImage("GamePadConfig", 4 + m_forceLevel);
		m_force.SetZoom(0.66f);
		m_force.SetPos(150, 50);
		m_force.SetAlpha(0);
		m_force.UpdateVertex();
	}

	void OnEntry(int offset) override
	{
		if (offset == 0)
		{
			m_forceLevel++;
			m_forceLevel %= 4;
			Basic::Sound::SeManager::Play("timeout2", 0);
			m_force.UseImage("GamePadConfig", 4 + m_forceLevel);
			__SetHapticStrength(ForceLevel[m_forceLevel]);
			Basic::Input::WriteJoystickButtonSetting();
			Basic::Haptic::Play(200);
		}
	}

	void OnInitSelect(int offset) override
	{
		OnSelectChanged(offset);
	}

	void Update(float deltaTime) override
	{
		m_hiderAlpha.Update(deltaTime);
		m_spAlpha.Update(deltaTime);
		m_sp.SetAlpha(m_spAlpha * m_hiderAlpha);
		m_force.SetAlpha(m_spAlpha * m_hiderAlpha);
	}

	void Draw() const override
	{
		m_sp.Draw();
		m_force.Draw();
	}
};

extern bool JoystickConnected();

Activities::Title::MenuStack::TitleJoystickConfig::TitleJoystickConfig(TitleMainMenu * mainMenu):
	m_mainMenu{ mainMenu },
	m_menu{
		std::vector<std::shared_ptr<Basic::Menu::MenuItem>>{
			std::make_shared<JSButtonSetting>(
				[this] {
					auto pGuide = std::make_unique<TitleJoystickGuide>(m_mainMenu);
					Hide();
					m_guide = pGuide.get();
					m_mainMenu->CallSubMenu(std::move(pGuide));
				}),
			std::make_shared<JSHapticSetting>()
		},0
	},
	m_matcher{
		[this] {m_menu.Down(); },
		[this] {m_menu.Up(); }
	}
{
	auto playSound = [] {
		Basic::Sound::SeManager::Play("timeout2", 0);
	};
	m_menu.SetOnDown(playSound);
	m_menu.SetOnUp(playSound);
	m_menu.SetEnabled(true);
}

void Activities::Title::MenuStack::TitleJoystickConfig::OnBackToThisShow()
{
	m_guide = nullptr;
	for (auto& p : m_menu)
		dynamic_cast<JSMenuItem&>(*p).Show();
}

void Activities::Title::MenuStack::TitleJoystickConfig::OnEntryToThis()
{
}

void Activities::Title::MenuStack::TitleJoystickConfig::OnDrawYSJD() const
{
	m_menu.Draw();
}

void Activities::Title::MenuStack::TitleJoystickConfig::OnUpdate(float deltaTime)
{
	m_taskList.Update(deltaTime);
	m_menu.Update(deltaTime);
	if (JoystickConnected() != m_connected)
	{
		m_connected = JoystickConnected();

		if (m_guide)
		{
			m_mainMenu->BackThisMenu();
			m_guide = nullptr;
		}

		if (!m_connected)
		{
			m_mainMenu->WannaBackThisMenu();
			Hide();
			m_taskList.AddTask([this] {
				m_mainMenu->BackThisMenu();
			}, 0.25f);
		}
	}
}

void Activities::Title::MenuStack::TitleJoystickConfig::OnEvent(const Basic::Input::Message & e)
{
	if (e.type == Basic::Input::Message::Type::KeyDown)
	{
		if (e.key == decltype(e.key)::Enter || e.key == decltype(e.key)::Shoot)
		{
			m_menu.Enter();
		}
		else if (e.key == decltype(e.key)::Esc || e.key == decltype(e.key)::Bomb)
		{
			Basic::Sound::SeManager::Play("exit", 0,120);
			m_mainMenu->WannaBackThisMenu();
			Hide();
			m_taskList.AddTask([this] {
				m_mainMenu->BackThisMenu();
			}, 0.25f);
		}
	}

	else if (e.type == Basic::Input::Message::Type::VecMotion)
	{
		m_matcher.OnEvent(e.vec.y);
	}
}

void Activities::Title::MenuStack::TitleJoystickConfig::Hide()
{
	for (auto& p : m_menu)
		dynamic_cast<JSMenuItem&>(*p).Hide();
}
