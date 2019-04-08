#pragma once
#include "BaseAfterEffect.h"
#include "DataActor.h"
#include "GLTexture.h"

namespace AfterEffect {
	class RollingBookPage :
		public AfterEffect::BaseAfterEffect
	{
	private:
		bool m_finished = false;
		double m_shaderTime = -0.5;
		Basic::DataActor m_data;
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_screenshot;
	public:
		RollingBookPage(int frame, int slowMotion, const std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>&);

		bool Killed() override;
		void Draw() const override;
		void Update() override;
	};
}