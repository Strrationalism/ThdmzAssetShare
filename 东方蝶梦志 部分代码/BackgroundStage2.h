#pragma once
#include "BaseBackground.h"
#include "GLCallList.h"
#include "GLTexture.h"
#include <vector>
#include "DataActor.h"
#include "TaskList.h"

namespace Background {
	class BackgroundStage2 :
		public Background::BaseBackground
	{
	private:
		struct Particle {
			double x, y, z;	//树左下角点的位置
			double alpha;	//透明度
			bool down;	//Alpha是否正在下降
			void Update(bool isGrass = false);

			inline bool operator < (const Particle& t) const {
				return z < t.z;
			}
		};

		std::vector<Particle> m_trees,m_tree2s,m_grasses,m_boxes;

		int m_partTimer = 0;

		double m_fogStart, m_fogEnd;
		double m_texFogAlpha;
		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		Basic::Video::LowerRenderer::GLCallList initAndDrawSky;
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_tree, m_cloud, m_kabe, m_tree2, m_dimian, m_noise64,m_grass,m_box;

		std::optional < Basic::Video::LowerRenderer::GLTexture> m_grassAndMoonFB;

		bool m_isNvdiaTrush;

		double m_modelView[16];

		double m_godRayAlpha;

		//void addPartToDrawCall(const Particle&) const;
		void addTreeToDrawCall(const Particle&) const;
		void addGrassToDrawCall(const Particle&) const;
		void addBoxToDrawCall(const Particle&) const;
		//void drawParticle(const Particle&) const;

		int m_timer = 0;
	public:
		BackgroundStage2();
		virtual void Draw() const override;
		virtual void Update() override;

		virtual void Init() override;

		virtual void Reset() override;
	};
}