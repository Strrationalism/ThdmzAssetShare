#pragma once
#include "BaseBackground.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "TaskList.h"
#include "DataActor.h"
namespace Background {

	using namespace std;
	using namespace Basic::Video;
	class BackgroundStage6 :
		public BaseBackground
	{
	private:
		int m_time;

		//Util
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		void drawImageFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const;
		void drawImageNegaFullScreen(GLuint tex, const float color[4], const float cameraCenter[2], float cameraZoom) const;

		//Sky
		void drawSky() const;
		double m_starCenterX, m_starSpeed;
		double m_dayLerp;	//如果是清晨则为1，夜晚则0
		shared_ptr<LowerRenderer::GLTexture> m_starSkyTexture,m_cloud;
		Renderer2D::Sprite2D m_sun,m_sunLight,m_sunLightCircle;
		

		//Cloud
		void preDrawCloud() const;
		void drawCloud() const;
		void updateCloud();

		double m_cloudY;
		double m_cloudSpeed;

		shared_ptr<LowerRenderer::GLTexture> m_cloudsTex;

		struct CloudParticle
		{
			float pos[3];
			float wh[2];
			float alpha;
			LowerRenderer::GLTexture::UVRect uvRect;
			void InsertToDrawOldCall() const;

			inline bool operator < (const CloudParticle& c) const 
			{
				return pos[2] < c.pos[2];
			}
		};
		std::vector<CloudParticle> m_cloudParts;

		shared_ptr<LowerRenderer::GLTexture> m_cloudAEColorBuf;

	public:
		BackgroundStage6();
		~BackgroundStage6();

		void Draw() const override;
		void Update() override;

		void Init() override;

		void Reset() override;

		void SkipToBossMode() override;
	};
}
