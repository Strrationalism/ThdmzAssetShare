#include "Precompiled.h"
#include "NameCard.h"
#include "Shaders.h"

static std::map<std::string,int> name2nameCardIndex = 
{
	{"Reisen",0},
	{"Tewi",1},
	{"Doremi",2},
	{"Daiyousei",3},
	{"DaiyouseiEx",3},
	{"Cirno",4},
	{"RReimu",5},
	{"RMarisa",6},
	{"Handan",7},
	{"AwanLjan",8},
	{"Alice",9}
};

Conversation::NameCard::NameCard(const std::string& charName)
{
	m_drawOnTop = true;
	m_died = false;

	m_nameCard.UseImage("GameUI/NameCards", name2nameCardIndex.at(charName));
	m_nameCard.SetPos(200, 100);
	m_nameCard.UpdateVertex();

	m_dataActor.Add(m_alpha, 1, 50, 5);

	m_taskList.AddTask([this]() {
		m_dataActor.Add(m_alpha, 0, 100, 5);
	}, 500);
}

void Conversation::NameCard::Update()
{
	m_dataActor.Update();
	m_taskList.Update();

	m_nameCard.SetAlpha(m_alpha);
}

void Conversation::NameCard::Draw() const
{
	if(m_alpha < 1 && m_alpha > 0)
		Basic::Shaders::GaussBlurW(32 * (1-m_alpha));
	m_nameCard.Draw();
	glUseProgram(0);
}

void Conversation::NameCard::Kill()
{
	if(m_alpha <= 0)
		m_died = true;
	else
	{
		m_dataActor.Add(m_alpha, 0, 50, 1);
		m_taskList.Clear();
		m_taskList.AddTask([this]() {
			m_died = true;
		},50);
	}
}
