#pragma once

class GameObject {
protected:
	bool m_died = true;
	bool m_visible = true;
	bool m_drawOnTop = false;
public:
	virtual ~GameObject() {}
	virtual void Update() = 0;
	virtual void Draw() const = 0;
	virtual void Kill() = 0;
	inline bool Died() {
		return m_died;
	}
	inline bool Visible() {
		return m_visible;
	}
	inline bool DrawOnTop() {
		return m_drawOnTop;
	}
};
