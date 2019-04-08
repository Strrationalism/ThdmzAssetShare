#pragma once
#include <vector>

class WarningLineSystem
{
private:
	struct WarningLine {
		double x1, y1, x2, y2, r, g, b;
		int time;
	};

	std::vector<WarningLine> m_lines;
	std::vector<double> m_verticles, m_colors;
public:
	void CreateLineCrossScreen(double x, double y, double angle, double r, double g, double b, int time);	//传入过线的一个点和一个角度，以及颜色值创建新的预警线
	void CreateLineFromPoint(double x, double y, double angle, double r, double g, double b, int time);
	void Update();
	inline void ClearWithFadeoutAnimation()
	{
		for (auto& l : m_lines)
			l.time = 25;
	}
	void Draw() const;
	inline void Clear() {
		m_lines.clear();
		m_verticles.clear();
		m_colors.clear();
	}
};
