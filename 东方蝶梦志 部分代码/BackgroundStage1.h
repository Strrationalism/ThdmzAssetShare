#pragma once
#include "BaseBackground.h"
#include "GLTexture.h"
#include "TaskList.h"
#include "DataActor.h"
#include "GLCallList.h"
#include "Sprite2D.h"

namespace Background {
	class BackgroundStage1 :
		public Background::BaseBackground
	{
	private:
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>  m_shan, m_shui, m_cloud, m_starSky, m_starSkyBlured;

		double m_modelView[16], m_projection[16];
		bool m_drawWaterLight;

		double m_fog_density;
		double m_fogColor[4];

		double m_waterOrIce;
		double m_waterFogMul;

		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		double m_cnt = 0;

		void DrawVSLightRef() const;
		void DrawVSLight() const;

		int m_timer = 0;

	public:
		BackgroundStage1();
		~BackgroundStage1() override;

		void Draw() const override;
		void Update() override;

		void Init() override;

		void Reset() override;
	};
}