#pragma once
#include "GameObjectWithDmslControl.h"
#include "Sprite2D.h"

class Sp2DObject :public GameObjectWithDmslControl
{
private:
	Basic::Video::Renderer2D::Sprite2D m_sp;
public:
	void Draw() const override;
	void Update() override;
	void Kill() override;
	inline Basic::Video::Renderer2D::Sprite2D& GetSprite() {
		return m_sp;
	}
	inline const Basic::Video::Renderer2D::Sprite2D& GetSprite() const {
		return m_sp;
	}
};
