#include "Precompiled.h"
#include "BossBgLayer.h"
#include "Game.h"
#include "DmslLib.h"
#include "DmslLoader.h"

Boss::BossBgLayer::Loc Boss::BossBgLayer::m_loc;
std::unique_ptr<Dmsl::DmslVirtualMachine> Boss::BossBgLayer::m_dvm;

void Boss::BossBgLayer::downloadData()
{
	m_x = m_dmsl.GetAttribute(m_loc.locX);
	m_y = m_dmsl.GetAttribute(m_loc.locY);

	m_alpha = m_dmsl.GetAttribute(m_loc.locAlpha);
	//m_sp.SetAlpha(1);
	m_angle = m_dmsl.GetAttribute(m_loc.locAngle);

	m_r = m_dmsl.GetAttribute(m_loc.locR);
	m_g = m_dmsl.GetAttribute(m_loc.locG);
	m_b = m_dmsl.GetAttribute(m_loc.locB);

	m_xZoom = m_dmsl.GetAttribute(m_loc.locXZoom);
	m_yZoom = m_dmsl.GetAttribute(m_loc.locYZoom);
}

void Boss::BossBgLayer::LoadScript()
{
	m_dvm.reset(nullptr);
	Dmsl::DmslLoader loader;
	loader.Include("Math");
	loader.IncludeMain("Dmsl/SpellBackground");
	m_dvm = loader.CompileUnique();
	Dmsl::LoadMathLibrary(*m_dvm);

	m_loc.locX = m_dvm->GetLoc("x");
	m_loc.locY = m_dvm->GetLoc("y");
	m_loc.locAngle = m_dvm->GetLoc("angle");
	m_loc.locAlpha = m_dvm->GetLoc("alpha");
	m_loc.locR = m_dvm->GetLoc("red");
	m_loc.locG = m_dvm->GetLoc("green");
	m_loc.locB = m_dvm->GetLoc("blue");
	m_loc.locXZoom = m_dvm->GetLoc("xZoom");
	m_loc.locYZoom = m_dvm->GetLoc("yZoom");

	m_dvm->Ready();
}

Boss::BossBgLayer::~BossBgLayer()
{
}

Boss::BossBgLayer::BossBgLayer(int dmslUnit, const std::string & texture, int blendMode) :m_dmsl(*m_dvm, (uint32_t)dmslUnit)
{
	m_texture.LoadImage(texture);
	m_died = false;
	m_visible = true;
	m_x = 0;
	m_y = 0;
	m_alpha = 0;

	m_texture.GetSize(m_w, m_h);
	m_blendMode = blendMode;
	//Log("Create BG:%x Dmsl:%x\n", this,m_dvm.get());
}

void Boss::BossBgLayer::Show()
{
	m_dataActor.Add(m_alpha2, 1, 40, 1);
}

void Boss::BossBgLayer::Hide()
{
	m_dataActor.Add(m_alpha2, 0, 20, 1);
}

void Boss::BossBgLayer::Update()
{
	m_dataActor.Update();
	m_dmsl.Call();
	downloadData();
}

void Boss::BossBgLayer::Draw() const
{
	if (m_alpha2 != 0) {
		glColor4d(m_r, m_g, m_b, m_alpha * m_alpha2);

		glRotated(m_angle, 0, 0, 1);
		glBindTexture(GL_TEXTURE_2D, m_texture);

		const double xEdge = WIDTH / 2 / m_xZoom;
		const double yEdge = HEIGHT / 2 / m_yZoom;

		switch (m_blendMode) {
		case 1:
			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
			break;
		case 2:
			glBlendFunc(GL_DST_COLOR, GL_ONE);
			break;
		}

		glBegin(GL_QUADS); {
			glTexCoord2d((m_x - xEdge) / m_w + 0.5, (m_y - yEdge) / m_h + 0.5);
			glVertex2d(-WIDTH / 2, -HEIGHT / 2);

			glTexCoord2d((m_x + xEdge) / m_w + 0.5, (m_y - yEdge) / m_h + 0.5);
			glVertex2d(WIDTH / 2, -HEIGHT / 2);

			glTexCoord2d((m_x + xEdge) / m_w + 0.5, (m_y + yEdge) / m_h + 0.5);
			glVertex2d(WIDTH / 2, HEIGHT / 2);

			glTexCoord2d((m_x - xEdge) / m_w + 0.5, (m_y + yEdge) / m_h + 0.5);
			glVertex2d(-WIDTH / 2, HEIGHT / 2);
		}
		glEnd();

		glLoadIdentity();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
}

void Boss::BossBgLayer::Kill()
{
	Hide();
	Game::GetGame().GetTaskList().AddTask([this] {
		m_died = true;
	}, 20);
}