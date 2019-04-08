#include "Precompiled.h"
#include "StageTitle.h"
#include "Game.h"
#include "DmslLoader.h"

static std::unique_ptr<Dmsl::DmslVirtualMachine> dvm;

struct DmslAttrLoc {
	int alpha, size, x, y,kill;
};
static DmslAttrLoc attrLoc;

void StageTask::StageTitle::StageTitleObj::Init()
{
	Dmsl::DmslLoader loader;
	loader.Include("Math");
	loader.IncludeMain("Dmsl/StageTitle");
	
	dvm = loader.CompileUnique();
	Dmsl::LoadMathLibrary(*dvm);

	attrLoc.alpha = dvm->GetLoc("alpha");
	attrLoc.size = dvm->GetLoc("size");
	attrLoc.x = dvm->GetLoc("x");
	attrLoc.y = dvm->GetLoc("y");
	attrLoc.kill = dvm->GetLoc("kill");

	dvm->Ready();
}

StageTask::StageTitle::StageTitleObj::StageTitleObj(int dmslUnit, int texUnit):
	m_dmslUnit(*dvm,dmslUnit)
{
	m_sp.UseImage("StageTitle",texUnit);
	m_died = false;
}

void StageTask::StageTitle::StageTitleObj::Update()
{
	m_dmslUnit.Call();
	m_sp.SetAlpha(m_dmslUnit.GetAttribute(attrLoc.alpha));
	m_sp.SetZoom(m_dmslUnit.GetAttribute(attrLoc.size));
	m_sp.SetPos(m_dmslUnit.GetAttribute(attrLoc.x), m_dmslUnit.GetAttribute(attrLoc.y));
	m_sp.UpdateVertex();

	if (m_dmslUnit.GetAttribute(attrLoc.kill) > 0)
		Kill();
}

void StageTask::StageTitle::StageTitleObj::Draw() const
{
	m_sp.DrawToFastBuffer();
}

void StageTask::StageTitle::StageTitleObj::Kill()
{
	m_died = true;
}

void StageTask::StageTitle::Create(Basic::CSVReader & c, const StageData &)
{
	int dmsl = c.PopInt();
	int tex = c.PopInt();
	m_obj = std::make_unique<StageTitleObj>(dmsl, tex);
}

void StageTask::StageTitle::Proc()
{
	m_finished = true;
	Game::GetGame().GetItemMgr().GetAll();
	Game::GetGame().GetUILayerGameObjectManager().Insert(m_obj.release());
}

bool StageTask::StageTitle::Finished()
{
	return m_finished;
}
