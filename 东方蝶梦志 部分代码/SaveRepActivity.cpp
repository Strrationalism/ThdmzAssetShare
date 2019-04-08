#include "NuzPrecompiled.h"
#include "SaveRepActivity.h"
#include <cctype>
#include "SecTweenData.h"
#include "GetPath.h"
#include <chrono>
#include "ActivityEngine.h"
#include "PlayerProfile.h"

using namespace std;
using namespace Basic::Menu;
using namespace Basic;

extern Basic::Video::Renderer2D::CtxFont scFont;

constexpr int XPos = -200;

const static auto SelectSeFunc = [] {
	Basic::Sound::SeManager::Play("timeout2", 0);
};

class IMECell : public MenuItem
{
private:
	Basic::Video::Renderer2D::Sprite2D m_spHigh, m_spLow;
	SecTweenData<float> m_y,m_alphaHigh,m_alphaLow,m_x,m_superMainAlpha;
	bool m_upperCase = false;
	const wchar_t m_upperChar, m_lowerChar;

	float m_timer = 0;

	bool m_selected = false;

	const std::function<void(wchar_t)> m_inputAction;
public:
	IMECell(wchar_t spHigh,wchar_t spLow,const std::function<void(wchar_t)>& input):
		m_upperChar{spHigh},
		m_lowerChar{ spLow },
		m_inputAction {input}
	{
		scFont.SetSprite2DToChar(spHigh, m_spHigh);
		scFont.SetSprite2DToChar(spLow, m_spLow);

		m_spHigh.SetPos(0, 0);
		m_spHigh.SetAlpha(0);
		m_spHigh.UpdateVertex();

		m_spLow.SetPos(0, 0);
		m_spLow.SetAlpha(0);
		m_spLow.UpdateVertex();

		ChangeCase();

		if (m_upperChar == m_lowerChar)
		{
			m_alphaHigh = 1;
			m_alphaLow = 1;
		}

		m_x = 0;

		m_superMainAlpha = 0;
	}

	void OnSelectChanged(int offset) override
	{
		m_selected = offset == 0;
		m_timer = 0;


		m_y.Tween(-offset * 42, 0.25f, 1);

	}

	void OnInitSelect(int offset) override
	{
		m_y = -offset * 42;
		OnSelectChanged(offset);
	}

	void OnEntry(int offset) override
	{
		wchar_t c = m_upperCase ? m_upperChar : m_lowerChar;
		if(offset == 0)
			m_inputAction(c);
	}

	void Update(float deltaTime) override
	{
		m_timer += deltaTime;
		const auto flashAlpha = m_selected ? sinf(m_timer * 16) * 0.5f + 0.5f : 1;

		m_superMainAlpha.Update(deltaTime);
		m_y.Update(deltaTime);
		m_x.Update(deltaTime);
		m_alphaLow.Update(deltaTime);
		m_alphaHigh.Update(deltaTime);

		const float mainAlpha = (100 - std::clamp(abs(m_y), 0.0f,100.0f)) / 100.0f;

		m_spHigh.SetPos(m_x, m_y);
		m_spHigh.SetAlpha(m_superMainAlpha * mainAlpha * m_alphaHigh * flashAlpha);
		m_spHigh.UpdateVertex();

		m_spLow.SetPos(m_x, m_y);
		m_spLow.SetAlpha(m_superMainAlpha * mainAlpha * m_alphaLow * flashAlpha);
		m_spLow.UpdateVertex();
	}

	void Draw() const override
	{
		m_spHigh.Draw();
		m_spLow.Draw();
	}

	void ChangeCase()
	{
		if (m_lowerChar != m_upperChar)
		{
			m_upperCase = !m_upperCase;

			m_alphaHigh.Tween(m_upperCase ? 1 : 0, 0.25f, 1);
			m_alphaLow.Tween(m_upperCase ? 0 : 1, 0.25f, 1);
		}
	}

	auto& GetX()
	{
		return m_x;
	}

	void Hide()
	{
		m_superMainAlpha.Tween(0, 0.25f, 1);
	}

	void Show()
	{
		m_superMainAlpha.Tween(1, 0.25f, 1);
	}
};

std::vector<std::shared_ptr<Basic::Menu::MenuItem>> Activities::SaveRepActivity::createIme()
{
	vector<shared_ptr<MenuItem>> ret;

	const std::function<void(wchar_t)> input = [this](wchar_t ch)
	{
		Basic::Sound::SeManager::Play("timeout", 0);

		const int xOffset = m_inputBox.size() * 28;
		
		auto chTup = std::make_unique<std::tuple<Basic::Video::Renderer2D::Sprite2D, wchar_t>>();
		get<1>(*chTup) = ch;

		if (ch != L'_')
		{
			auto& sp = get<0>(*chTup);
			scFont.SetSprite2DToChar(ch, sp);
			sp.SetPos(XPos + xOffset, 0);
			sp.UpdateVertex();
		}
		m_inputBox.push_back(std::move(chTup));

		m_ime.For<IMECell>([xOffset](IMECell& i) {
			i.GetX().Tween(XPos + xOffset + 28, 0.25f, 1);
		});

		if (m_inputBox.size() >= 8)
		{
			m_ime.SetEnabled(false);
			m_ime.For<IMECell>([](IMECell& i) {
				i.Hide();
			});
		}
	};

	ret.push_back(static_pointer_cast<MenuItem>(make_shared<IMECell>(L'_', L'_', input)));

	for (wchar_t ch = L'A'; ch <= L'Z'; ++ch)
	{
		ret.push_back(static_pointer_cast<MenuItem>(make_shared<IMECell>(ch,(wchar_t)tolower(ch), input)));
	}

	for (wchar_t ch = L'0'; ch <= L'9'; ++ch)
	{
		ret.push_back(static_pointer_cast<MenuItem>(make_shared<IMECell>(ch, ch, input)));
	}

	return ret;
}

std::wstring Activities::SaveRepActivity::getInputBox()
{
	std::wstring inputBox;
	for (const auto& p : m_inputBox)
	{
		inputBox += get<1>(*p);
	}
	return inputBox;
}

std::wstring Activities::SaveRepActivity::getDateString()
{
	tm t;
	auto timer = time(nullptr);
	localtime_s(&t, &timer);
	
	return to_wstring(t.tm_year+1900) + L'.' + to_wstring(t.tm_mon+1) + L'.' + to_wstring(t.tm_mday) + L' ' +
		to_wstring(t.tm_hour) + L'-' + to_wstring(t.tm_min) + L'-' + to_wstring(t.tm_sec);
}

Activities::SaveRepActivity::SaveRepActivity(MovieData::MovieData& movieData, std::function<void()>&& drawExt, std::function<void(float)>&& updateExt,std::function<void()>&& afterSave):
	m_drawExt{move(drawExt)},
	m_afterSave{move(afterSave)},
	m_updateExt{ move(updateExt) },
	m_ime{ createIme() },
	m_movie{ movieData },
	m_udvm{ [this] {m_ime.Down(); },[this] {m_ime.Up(); } },
	m_lrvm{
		[this] {
			m_ime.SetEnabled(true);
			if (!m_inputBox.empty())
			{
				m_inputBox.pop_back();

				Basic::Sound::SeManager::Play("timeout", 0);

				const int xOffsetCell = XPos + m_inputBox.size() * 28;
				m_ime.For<IMECell>([xOffsetCell](IMECell& i) {
					i.GetX().Tween(xOffsetCell, 0.25f, 1);
				});

				m_ime.For<IMECell>([](IMECell& i) {
					i.Show();
				});
				
			}
		},
		[this] {
			if (m_inputBox.size() < 8)
			{
				m_ime.Enter();
				m_ime.SelectItem(0);
			}
		}
}
{
	m_ime.SetOnDown(SelectSeFunc);
	m_ime.SetOnUp(SelectSeFunc);
	m_ime.SetEnabled(true);
	m_ime.SetClamp(false);

	m_mainAlpha = 0;
	m_mainAlpha.Tween(1, 0.25f, 1);

	m_title.UseImage("GameUI/PauseActivity", 7);
	m_title.SetPos(0, -200);
	m_title.SetZoom(1.25);
	m_title.SetAlpha(0);
	m_title.UpdateVertex();

	m_hint.UseImage("GameUI/PauseActivity", 8);
	m_hint.SetPos(-500, 200);
	m_hint.SetAlpha(0);
	m_hint.UpdateVertex();

	m_ime.For<IMECell>([](IMECell& i) {
		i.GetX() = XPos;
	});

	//读入并使用已有的机签
	const auto playerName = SaveSystem::PlayerProfile::Get().GetPlayerName();
	for (auto ch : playerName)
	{
		const int xOffset = m_inputBox.size() * 28;

		auto chTup = std::make_unique<std::tuple<Basic::Video::Renderer2D::Sprite2D, wchar_t>>();
		get<1>(*chTup) = ch;

		if (ch != L'_')
		{
			auto& sp = get<0>(*chTup);
			scFont.SetSprite2DToChar(ch, sp);
			sp.SetPos(XPos + xOffset, 0);
			sp.SetAlpha(0);
			sp.UpdateVertex();
		}
		m_inputBox.push_back(std::move(chTup));

		m_ime.For<IMECell>([xOffset](IMECell& i) {
			i.GetX() = XPos + xOffset + 28;
		});
	}

	if (m_inputBox.size() >= 8)
	{
		m_ime.SetEnabled(false);
		m_ime.For<IMECell>([](IMECell& i) {
			i.Hide();
		});
	}
	else
		m_taskList.AddTask([this] {
			m_ime.For<IMECell>([](IMECell& i) { i.Show(); });
		}, 0.5f);
}

void Activities::SaveRepActivity::OnUpdate(float deltaTime)
{
	m_taskList.Update(deltaTime);
	m_updateExt(deltaTime);
	m_ime.Update(deltaTime);
	m_mainAlpha.Update(deltaTime);
	m_title.SetAlpha(m_mainAlpha);
	m_hint.SetAlpha(m_mainAlpha);

	for (const auto& p : m_inputBox)
		get<0>(*p).SetAlpha(m_mainAlpha);
}

void Activities::SaveRepActivity::OnDraw() const
{
	m_drawExt();

	m_title.Draw();
	m_hint.Draw();
	m_ime.Draw();

	for (const auto& p : m_inputBox)
		if(get<1>(*p) != L'_')
			get<0>(*p).Draw();
}

void Activities::SaveRepActivity::OnEvent(const Basic::Input::Message & e)
{
	if (!m_reciveEvent) return;

	if (e.type == decltype(e.type)::VecMotion)
	{
		if (e.vec.x == 0)
			m_lrvm.OnEvent(0);

		if (e.vec.y == 0)
			m_udvm.OnEvent(0);

		if (std::abs(e.vec.x) > std::abs(e.vec.y))
			m_lrvm.OnEvent(e.vec.x);
		else
			m_udvm.OnEvent(e.vec.y);
	}
	else if (e.type == decltype(e.type)::KeyDown)
	{
		if (e.key == decltype(e.key)::Shoot || e.key == decltype(e.key)::Enter)
		{
			if (!m_inputBox.empty())
			{
				m_reciveEvent = false;

				Basic::Sound::SeManager::Play("cardget", 0,120);

				const auto inputBox = getInputBox();
				wcscpy_s(m_movie.head.playerName, inputBox.c_str());

				SaveSystem::PlayerProfile::Get().SetPlayerName(inputBox.c_str());

				const std::wstring fileName = 
					SaveSystem::GetReplayPath().wstring()+ L'\\' + 
					inputBox + L' ' + getDateString() +
					L".rep";
				const auto path = std::filesystem::path(fileName);
				std::ofstream out(path.string(), std::ios::binary);
				MovieData::SaveMovie(m_movie, out);
				out.close();

				

				m_ime.For<IMECell>([](IMECell& i) {i.Hide(); });
				m_mainAlpha.Tween(0, 0.25f, 1);

				m_taskList.AddTask([this] {
					m_afterSave();
					Basic::ActivityEngine::ActivityEngine::GetActivityEngine().Return();
				}, 0.3F);
			}
		}
		else if (e.key == decltype(e.key)::Slow)
		{
			SelectSeFunc();
			m_ime.For<IMECell>([](IMECell& i) {i.ChangeCase(); });
		}
	}
}
