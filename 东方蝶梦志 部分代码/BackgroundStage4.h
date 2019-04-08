#pragma once
#include "BaseBackground.h"
#include "TaskList.h"
#include "GLTexture.h"
#include "DataActor.h"
class BackgroundStage4 :
	public Background::BaseBackground
{
private:
	uint64_t m_time;
	double m_glassDepth;
	double m_blueMul;
	Basic::DataActor m_dataActor;
	Basic::TaskList m_taskList;

	std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_grass,m_shui,m_screenShotForGrass,m_clouds;
public:
	BackgroundStage4();
	void Draw() const override;
	void Update() override;

	void Init() override;

	void Reset() override;
	void SkipToBossMode() override;
};
