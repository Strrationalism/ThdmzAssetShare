#include "Precompiled.h"
#include "WarningLineSystem.h"
#include "Globals.h"
#include "LineMath.h"



struct Point
{
	double x, y;
};

inline static Point createLine(Point start, double length, double angle)
{
	const float rad = angle * M_PI / 180;
	return Point{ start.x + length * cos(rad), start.y + length * sin(rad) };
}

inline void clampToScreen(Point& pt1,const Point& pt2) 
{
	/*double t[4] = {
		Clamp(0,1,(640 - pt1.x) / (pt2.x - pt1.x)),
		Clamp(0, 1, (-640 - pt1.x) / (pt2.x - pt1.x)),
		Clamp(0, 1, (-320 - pt1.y) / (pt2.y - pt1.y)),
		Clamp(0, 1, (320 - pt1.y) / (pt2.y - pt1.y))
	};
	for (auto p : t) {
		pt1.x = Mix(pt1.x, pt2.x, p);
		pt1.y = Mix(pt1.y, pt2.y, p);
	}*/
}

void WarningLineSystem::CreateLineCrossScreen(double x, double y, double angle, double r, double g, double b,int time)
{
	Point p = { x,y };
	Point start = createLine(p, MAX_LINE_IN_SCREEN, angle - 180);
	//start = p;
	CreateLineFromPoint(start.x, start.y, angle, r, g, b,time);
}

void WarningLineSystem::CreateLineFromPoint(double x, double y, double angle, double r, double g, double b,int time)
{
	Point p1 = { x,y };
	Point p2 = createLine(p1, MAX_LINE_IN_SCREEN * 2, angle);
	clampToScreen(p2,p1);

	m_lines.push_back(WarningLine{
		p1.x,p1.y,
		p2.x,p2.y,
		r,g,b,time
	});
}



void WarningLineSystem::Update()
{
	m_verticles.clear();
	m_colors.clear();

	for (auto& p : m_lines) {
		double alpha = Clamp(0,25,p.time) / 25.0;
		m_verticles.push_back(p.x1);
		m_verticles.push_back(p.y1);
		m_verticles.push_back(p.x2);
		m_verticles.push_back(p.y2);
		m_colors.push_back(p.r);
		m_colors.push_back(p.g);
		m_colors.push_back(p.b);
		m_colors.push_back(alpha);

		m_colors.push_back(p.r);
		m_colors.push_back(p.g);
		m_colors.push_back(p.b);
		m_colors.push_back(alpha);
		p.time--;
	}

	m_lines.erase(
		std::remove_if(m_lines.begin(), m_lines.end(), [](auto& line) {
		return line.time <= 0;
	}
		),
		m_lines.end()
	);
}

#include "glew.h"
void WarningLineSystem::Draw() const
{
	if (!m_verticles.empty()) {
		glLineWidth(0.5);


		glDisable(GL_TEXTURE_2D);
		glEnableClientState(GL_COLOR_ARRAY);

		glVertexPointer(2, GL_DOUBLE, 0, m_verticles.data());
		glColorPointer(4, GL_DOUBLE, 0, m_colors.data());
		glDrawArrays(GL_LINES, 0, m_verticles.size() / 2);

		glDisableClientState(GL_COLOR_ARRAY);
		glEnable(GL_TEXTURE_2D);
	}
}
