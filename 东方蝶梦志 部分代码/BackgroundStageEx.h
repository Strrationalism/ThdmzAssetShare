#pragma once
#include "BaseBackground.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "TaskList.h"
#include "DataActor.h"
namespace Background {

	using namespace std;
	using namespace Basic::Video;
	class BackgroundStageEx :
		public BaseBackground
	{
	private:
		Basic::DataActor m_dataActor;

		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_snowTex,m_shan, m_shui, m_bk,m_jb;

		struct SnowParticle
		{
			float pos[3];
			float r, g, b ,a;
			float wh[2];
			
			float speed;
			float rotateSpeed;
			float rotate;

			LowerRenderer::GLTexture::UVRect uvRect;

			int frames = 0;

			void Update();
			void ShadowUpdate();
			void InsertToDrawCall() const;
		};

		std::vector<SnowParticle> m_parts,m_partShadow;

		void drawImageFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const;

		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_pencilFBO;

		int m_time;
		bool m_isNvidiaTrust;

		//全屏雪渐变效果
		void drawSnow() const;
	public:
		BackgroundStageEx();
		~BackgroundStageEx();

		void Draw() const override;
		void Update() override;

		void Init() override;

		void Reset() override;
	};
}
