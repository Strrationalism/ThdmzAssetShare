#pragma once
#include "BaseBackground.h"
#include "GLCallList.h"
#include "GLTexture.h"
#include <vector>
#include "Sprite2D.h"
#include "DataActor.h"
#include "TaskList.h"

namespace Background {
	class BackgroundStage5 :
		public Background::BaseBackground
	{
	private:

		double m_fogStart, m_fogEnd;
		double m_texFogAlpha;
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		Basic::Video::LowerRenderer::GLCallList initAndDrawSky;
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_cloud, m_layer, m_layer3, m_birds, m_birds_blur, m_starSky;
		double m_starSkyAlpha, m_birdsAlpha, m_moonAlpha;
		double m_m_birdsAlphaBeats;

		Basic::Video::Renderer2D::Sprite2D m_moon;

		int m_timer = 0;
	public:
		BackgroundStage5();
		virtual void Draw() const override;
		virtual void Update() override;

		virtual void Init() override;

		virtual void Reset() override;

		void SkipToBossMode() override;
	};
}