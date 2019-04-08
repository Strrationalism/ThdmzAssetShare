#include "Precompiled.h"
#include "ConversationLogic.h"
#include "DmslLoader.h"
#include "CSVReader.h"
#include "NameCard.h"
#include "Game.h"

void Conversation::ConversationLogicDisplay::Draw() const
{
	m_logic->Draw();
}

void Conversation::ConversationLogicDisplay::Update()
{
}

void Conversation::ConversationLogicDisplay::Kill()
{
	m_died = true;
}

static bool exist = false;
bool Conversation::ConversationLogic::Exist()
{
	return exist;
}

Conversation::ConversationLogic::ConversationLogic(bool clickEnabled) 
	:m_clickEnabled(clickEnabled) 
{
	
}

Conversation::ConversationLogic::~ConversationLogic()
{
	exist = false;
}

void Conversation::ConversationLogic::SetLHObject(ConversationStandImage * left, ConversationStandImage * right)
{
	m_left = left;
	m_right = right;
}

bool Conversation::ConversationLogic::Update()
{
	exist = true;
	m_tasks.Update();

	m_delayer.Update();

	for (auto& p : m_spriteString)
		p->Update();

	auto nend = std::remove_if(m_spriteString.begin(), m_spriteString.end(), [](auto& p) {
		return p->Died();
	});

	m_spriteString.erase(nend, m_spriteString.end());

	int frameTime = 0;

	while (m_thisTaskFinished) {// || m_skipping) {
		//if(m_skipping) m_tasks.DoAllNow();

		if (m_script.empty())
			break;

		switch (m_script.front().cmd) {
		case ConversationScript::Script::Event::Cmd::ColorB:
			m_nowB = Mix(m_script.front().arg,1,0.8);
			break;
		case ConversationScript::Script::Event::Cmd::ColorG:
			m_nowG = Mix(m_script.front().arg,1,0.8);
			break;
		case ConversationScript::Script::Event::Cmd::ColorR:
			m_nowR = Mix(m_script.front().arg,1,0.8);
			break;
		case ConversationScript::Script::Event::Cmd::Program:
			m_nowProgram = (int)m_script.front().arg;
			break;
		case ConversationScript::Script::Event::Cmd::WaitTime:
			m_thisTaskFinished = false;
			m_tasks.AddTask([this]() {
				this->m_thisTaskFinished = true;
			}, (int)m_script.front().arg);
			break;
		case ConversationScript::Script::Event::Cmd::Cls:
			m_nowX = -400;
			m_nowY = 200;
			if (!this->m_spriteString.empty()) {
				m_thisTaskFinished = false;
				for (auto& p : m_spriteString)
					p->Kill();
				m_tasks.AddTask([this]() {
					m_thisTaskFinished = true;
				}, 10);
			}
			break;
		case ConversationScript::Script::Event::Cmd::WaitClick:
			m_thisTaskFinished = false;
			//m_skipping = false;
			m_clickTask = [this]() {
				this->m_tasks.Clear();
				m_thisTaskFinished = true;
			};
			m_tasks.AddTask([this]() {
				this->m_thisTaskFinished = true;
			}, (int)m_script.front().arg);
			break;
		case ConversationScript::Script::Event::Cmd::Print: {
			m_thisTaskFinished = false;
			//auto conversation = Str2WStr(m_scriptLoader->GetText((int)m_script.front().arg));
			for (wchar_t chr : m_scriptLoader->GetText((int)m_script.front().arg)) {
				if (chr == 32) continue;
				m_spriteString.push_back(std::make_unique<ConversationWord>());
				m_spriteString.back()->Create(chr, m_nowX, m_nowY, m_nowR, m_nowG, m_nowB, m_nowProgram, frameTime);

				auto size = GetCtxFont1().GetSize(chr);
				m_nowX += size.first;
				m_nowX -= 2;
				frameTime += 1;
			}
			frameTime -= 1;
			m_tasks.AddTask([this]() {
				m_thisTaskFinished = true;
			}, frameTime);

			m_clickEnabled = true;

			m_clickTask = [this]() {
				m_tasks.Clear();
				m_thisTaskFinished = true;
				m_clickEnabled = false;
				for (auto& p : m_spriteString) {
					p->EndEffect();
				}
				m_tasks.AddTask([this]() {
					m_clickEnabled = true;
				}, 10);
			};
			break;
		}
		case ConversationScript::Script::Event::Cmd::Br:
			m_nowX = -400;
			m_nowY += 32;
			break;
		case ConversationScript::Script::Event::Cmd::Act:
			if (m_script.front().arg == ConversationScript::Left)
				m_left->Act();
			else
			{
				m_right->Act();
				if ((!m_nameCard) && m_nameCardEnabled)
				{
					m_nameCard = new NameCard{ m_scriptLoader->GetRightLHName() };
					Game::GetGame().GetUILayerGameObjectManager().Insert(m_nameCard);
				}
			}
			break;
		case ConversationScript::Script::Event::Cmd::Una:
			if (m_script.front().arg == ConversationScript::Left)
				m_left->Unact();
			else
				m_right->Unact();
			break;
		case ConversationScript::Script::Event::Cmd::Hide:
			if (m_script.front().arg == ConversationScript::Left)
				m_left->Hide();
			else
				m_right->Hide();
			break;
		case ConversationScript::Script::Event::Cmd::SetLNum:
			m_left->SetLHNum((int)m_script.front().arg);
			break;
		case ConversationScript::Script::Event::Cmd::SetRNum:
			m_right->SetLHNum((int)m_script.front().arg);
			break;
		case ConversationScript::Script::Event::Cmd::BossBGM:
			if (Game::GetGame().GetStageData()->bossBgm != -1)
				Basic::Sound::BgmPlayer::Play(Game::GetGame().GetStageData()->bossBgm);
			break;
		case ConversationScript::Script::Event::Cmd::StopBGM:
			Basic::Sound::BgmPlayer::Stop(true);
			break;
		default:
			Crash(L"Conversation Bytecode Crash!");
			break;
		}

		//		if(m_skipping) m_tasks.DoAllNow();

		if (!m_script.empty()) m_script.pop();
		else break;
	}

	if (m_script.empty() && m_nameCard)
	{
		m_nameCard->Kill();
		m_nameCard = nullptr;
	}

	return !m_script.empty();
}

void Conversation::ConversationLogic::Draw() const
{
	for (auto& p : m_spriteString)
		p->Draw();
}

void Conversation::ConversationLogic::UseScript(ConversationScript::Script& s, const std::string& charName)
{
	m_scriptLoader = &s;
	s.GetCommand(m_script);
}

std::unique_ptr<Dmsl::DmslVirtualMachine> Conversation::ConversationWord::m_textEffectProgram;
int  Conversation::ConversationWord::m_locX, Conversation::ConversationWord::m_locY, Conversation::ConversationWord::m_locR, Conversation::ConversationWord::m_locG, Conversation::ConversationWord::m_locB,
Conversation::ConversationWord::m_locAfterFrameTime;

void Conversation::ConversationWord::Init()
{
	Dmsl::DmslLoader loader;
	loader.Include("Math");
	loader.IncludeMain("Dmsl/ConversationTextEffect");
	m_textEffectProgram = std::move(loader.CompileUnique());
	Dmsl::LoadMathLibrary(*m_textEffectProgram);
	m_locX = m_textEffectProgram->GetLoc("x");
	m_locY = m_textEffectProgram->GetLoc("y");
	m_locB = m_textEffectProgram->GetLoc("b");
	m_locG = m_textEffectProgram->GetLoc("g");
	m_locR = m_textEffectProgram->GetLoc("r");
	m_locAfterFrameTime = m_textEffectProgram->GetLoc("afterFrameTime");

	m_textEffectProgram->Ready();
}

void Conversation::ConversationWord::Update()
{
	if (m_afterFrameTime > 1) --m_afterFrameTime;
	else  if (m_afterFrameTime == 1) {
		m_dataActor.Add(m_alpha, 1, 10, 1);
		m_afterFrameTime = 0;
	}

	m_program->Call();
	m_sprite.SetPos(m_program->GetAttribute(m_locX), m_program->GetAttribute(m_locY));
	m_sprite.SetColorFliter(m_program->GetAttribute(m_locR), m_program->GetAttribute(m_locG), m_program->GetAttribute(m_locB));
	m_dataActor.Update();

	m_sprite.SetAlpha(m_alpha);
	m_sprite.UpdateVertex();
}

void Conversation::ConversationWord::Create(wchar_t word, double x, double y, double r, double g, double b, int program, int afterFrameTime)
{
	m_afterFrameTime = afterFrameTime;
	m_program.emplace(*m_textEffectProgram, program);

	GetCtxFont1().SetSprite2DToChar(word, m_sprite);
	//SDL_FreeSurface(sur);

	if (m_afterFrameTime == 0) {
		m_dataActor.Add(m_alpha, 1, 10, 1);
	}

	m_program->SetAttribute(m_locX, x);
	m_program->SetAttribute(m_locY, y);
	m_program->SetAttribute(m_locR, r);
	m_program->SetAttribute(m_locG, g);
	m_program->SetAttribute(m_locB, b);
	m_program->SetAttribute(m_locAfterFrameTime, afterFrameTime);
}

void Conversation::ConversationWord::Kill()
{
	m_killed = true;
}

std::map<std::string, int>  Conversation::ConversationStandImage::m_ldData;
void Conversation::ConversationStandImage::Init()
{
	Basic::CSVReader csv;
	csv.Load("LdData.csv");

	while (!csv.IsLastLine()) {
		auto s = csv.PopString();
		auto i = csv.PopInt();
		m_ldData[s] = i;
		csv.NextLine();
	}
}

void Conversation::ConversationStandImage::SetXPos(double actX, double unactX, double hideX)
{
	m_actX = actX;
	m_unactX = unactX;
	m_x = m_hideX = hideX;
	m_died = false;
}

#define LH_FADETIME 60

void Conversation::ConversationStandImage::Act()
{
	m_state = ACT;
	m_dataActor.Add(m_alpha[m_usingSp], 1, LH_FADETIME, 1);
	m_dataActor.Add(m_x, m_actX, LH_FADETIME, 1);
}

void Conversation::ConversationStandImage::Unact()
{
	m_state = UNACT;
	m_dataActor.Add(m_alpha[m_usingSp], 0.5, LH_FADETIME, 1);
	m_dataActor.Add(m_x, m_unactX, LH_FADETIME, 1);
}

void Conversation::ConversationStandImage::Hide()
{
	m_state = HIDDEN;
	m_dataActor.Add(m_alpha[m_usingSp], 0, LH_FADETIME, 1);
	m_dataActor.Add(m_x, m_hideX, LH_FADETIME, 1);
}

void Conversation::ConversationStandImage::SetLHNum(int num)
{
	m_dataActor.Add(m_alpha[m_usingSp], 0, LH_FADETIME, 1);
	m_usingSp = getAnotherSp();
	m_dataActor.Add(m_alpha[m_usingSp], m_alpha[getAnotherSp()], LH_FADETIME, 1);
	m_sp.at(m_usingSp).UseImage(m_textureName, num);
}

void Conversation::ConversationStandImage::SetCharater(const std::string & charName)
{
	m_textureName = "Charater/" + charName;
	m_sp.at(0).UseImage(m_textureName, 0);
	m_sp.at(1).UseImage(m_textureName, 0);

	m_y = m_ldData.at(charName);
}

void Conversation::ConversationStandImage::Update()
{
	m_dataActor.Update();
	m_sp.at(0).SetPos(m_x, m_y);
	m_sp.at(1).SetPos(m_x, m_y);
	m_sp.at(0).SetAlpha(m_alpha[0]);
	m_sp.at(1).SetAlpha(m_alpha[1]);

	for (auto& i : m_sp)
		i.SetZoom(1.2);

	m_sp.at(0).UpdateVertex();
	m_sp.at(1).UpdateVertex();
}

void Conversation::ConversationStandImage::Draw() const
{
	m_sp.at(0).Draw();
	m_sp.at(1).Draw();
}

void Conversation::ConversationStandImage::Kill()
{
	Hide();
	Game::GetGame().GetTaskList().AddTask([this]() {
		m_died = true;
	}, LH_FADETIME);
}