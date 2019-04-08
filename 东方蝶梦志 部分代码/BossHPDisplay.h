#pragma once
#include "GameObject.h"
#include "DataActor.h"
#include "GLCallList.h"
#include "GLShader.h"
#include <memory>
#include "Sprite2D.h"
#include "Collision.h"

namespace Boss {
	class BossHPDisplay :
		public GameObject
	{
	private:
		static std::optional<Basic::Video::LowerRenderer::GLCallList> m_drawShell;
		static Basic::Video::LowerRenderer::GLShader m_shader;
		double m_hp = 0;
		Basic::DataActor m_dataActor;

		double m_alpha;

		Collision::Circle m_coll;

		struct HPPoint {
			double hp;
			double x, y;
			std::unique_ptr<Basic::Video::Renderer2D::Sprite2D> sp;
		};
		std::vector<HPPoint> m_hpPoints;

		bool m_playerCover = false;
	public:
		BossHPDisplay();
		static void Init();
		void Draw() const override;
		void Update() override;
		void Kill() override;

		inline void SetHP(double hp) {	//·¶Î§0~1
			//GameObject::GetDataActor().Clear();
			//GameObject::GetDataActor().Add(m_hp, hp, 5, 2);
			if (hp < m_hp) {
				m_hp = hp;

				m_dataActor.Add(m_hp, hp, 25, 1);

				m_hpPoints.erase(std::remove_if(m_hpPoints.begin(), m_hpPoints.end(), [hp](auto& i) {
					return i.hp >= hp;
				}), m_hpPoints.end());
			}
		}

		void SetHPPoint(double hp);

		inline void Clear() {
			m_hpPoints.clear();
		}

		inline void FadeToFull() {
			m_dataActor.Add(m_hp, 1, 60, 1);
		}

		inline void SetPos(double x, double y) {
			m_coll.x = x;
			m_coll.y = y;
		}

		inline void SetVisible(bool b) {
			m_visible = b;
		}
	};
}