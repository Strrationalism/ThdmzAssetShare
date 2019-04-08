#pragma once
#include "BaseBackground.h"
#include "GLTexture.h"
#include "GLShader.h"
#include "GLTexture.h"
#include "TaskList.h"
#include "DataActor.h"

namespace Background {
	class BackgroundStage3 :
		public Background::BaseBackground
	{
	private:

		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_bk1, m_flamer, m_sTex, m_cloud ,m_gs1,m_gs2;
		int time;

		struct Particle {
			int number;
			double zDepth;
			inline bool operator < (const Particle& p) const {
				return zDepth < p.zDepth;
			}

			inline void Update() {
				zDepth += 0.005;
			}

			void Draw(Basic::Video::LowerRenderer::GLTexture*) const;
		};
		std::vector<Particle> m_particles;

		enum {
			Stage, Boss
		}m_scene;

		double m_gauss;
		double m_boss_roomlight = 0.0;
		double m_global_alpha;	//全局Alpha
		bool m_switch_particle;	//是否出粒子（壁画）
		double m_frameAlpha;	//火焰Alpha

		double m_playerX, m_playerY;

		Basic::TaskList m_taskList;
		Basic::DataActor m_dataActor;

		void stageDraw() const;
		void bossDraw() const;

		void stageUpdate();

	public:
		BackgroundStage3();
		~BackgroundStage3();

		void Draw() const override;
		void Update() override;
		void UpdateWithoutGame() override;

		void Init() override;

		void Reset() override;

		void SkipToBossMode() override;
	};
}
