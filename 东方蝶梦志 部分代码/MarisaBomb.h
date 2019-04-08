#pragma once
#include "GameObject.h"
#include "ImageCacheManager.h"
#include "Collision.h"
#include "TaskList.h"
namespace Char
{
	namespace Marisa
	{
		class MarisaBomb :
			public GameObject
		{
		private:
			struct Circle {
				double height;
				double startX;
				double x;
			};

			Basic::TaskList m_taskList;
			Basic::DataActor m_dataActor;

			std::vector<Circle> m_circles;

			bool m_flip;
			double m_width = 0;

			const bool m_usePlayerPos;
			const double m_x, m_y,m_height,m_angle;

			std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_mask, m_perlin, m_circle;

			double m_marisaBomb_r, m_marisaBomb_g, m_marisaBomb_b;

			void checkToKillPlayer(const Collision::Rectangle&);
			void buildColl(double length,double w);
		public:
			MarisaBomb(bool flip,bool usePlayerPos = true,double x=0,double y=0,double height=480,double angle = 0);
			~MarisaBomb();
			void Update() override;
			void Draw() const override;
			void Kill() override;

			static void SetPowerData(double);
		};
	}
}