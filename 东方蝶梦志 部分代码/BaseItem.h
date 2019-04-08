#pragma once
#include "Sprite2D.h"
#include "Collision.h"
#include "Globals.h"
namespace Item {
	class BaseItem {
	private:
		
		Collision::Circle m_circle;

		int m_getLimiter;

		bool m_getting = false;
		double m_getSpeed = 13;
		double m_gettingLength = 0;
		int m_timer = 0;
		bool m_died = false;
		bool m_negaX;

		double m_ySpd, m_xSpd;

		int m_blendMode = Basic::Video::Renderer2D::FASTBUFFER_BLEND_NORMAL;
	protected:
		Basic::Video::Renderer2D::Sprite2D m_itemImage;

		void SetItemImage(int num);
		inline void SetCollisionRadius(double radius) {
			m_circle.radius = radius;
		}

		inline void SetFastBufferTarget(int fastBuffer)
		{
			m_blendMode = fastBuffer;
		}

		inline void Kill() {
			m_died = true;
		}

		bool m_drawOnTop = false;
	public:
		virtual ~BaseItem() {}
		void Create(double x, double y, double ySpd, double xSpd);
		virtual void Update();

		inline bool DrawOnTop() {
			return m_drawOnTop;
		}

		inline void Draw() const {
			if (m_circle.y >= -HEIGHT / 2 - m_circle.radius / 2)
				m_itemImage.DrawToFastBuffer(m_blendMode);
		}

		void Get(bool itemGetLine = true);

		virtual void OnGot() = 0;
		virtual void OnCreate() = 0;

		inline bool Died() {
			return m_died;
		}
	};
}
