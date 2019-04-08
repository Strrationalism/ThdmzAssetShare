#pragma once
#include "glew.h"
#include "Sprite2D.h"
#include "Matrix.h"

namespace Collision {
	struct Circle {
		double x, y, radius;
	};

	struct Rectangle {
		double x, y,		// 矩形中心点坐标
			width, height;
		double rotate;		// 旋转的角度
	};

	inline bool CheckC2C(const Circle& c1, const Circle& c2) {
		double w = c1.x - c2.x;
		double h = c1.y - c2.y;
		double r = c1.radius + c2.radius;
		return r*r >= w*w + h*h;
	}

	inline void DrawCircle(const Circle& x) {
		//glLoadIdentity();
		//glActiveTexture(GL_TEXTURE0);
		glDisable(GL_TEXTURE_2D);
		glBegin(GL_QUADS);
		glColor4d(1, 1, 0, 0.5); glVertex2d(x.x - x.radius, x.y - x.radius);
		glColor4d(1, 1, 0, 0.5); glVertex2d(x.x + x.radius, x.y - x.radius);
		glColor4d(1, 1, 0, 0.5); glVertex2d(x.x + x.radius, x.y + x.radius);
		glColor4d(1, 1, 0, 0.5); glVertex2d(x.x - x.radius, x.y + x.radius);
		glEnd();
		glEnable(GL_TEXTURE_2D);
	}

	inline void DrawRectangle(const Rectangle& rect)
	{
		double verticles[] = 
		{
			-rect.width / 2,-rect.height / 2,
			-rect.width / 2,rect.height / 2,
			rect.width / 2,rect.height / 2,
			rect.width / 2,-rect.height / 2
		};



		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glBegin(GL_QUADS);
		for (int i = 0; i < 4; ++i)
		{
			double rx, ry;
			Basic::Math::Matrix::RotateVec2_2D(verticles[2 * i], verticles[2 * i + 1], -rect.rotate * M_PI / 180,&rx,&ry);
			glColor4d(1, 1, 0, 1); glVertex2d((rx + rect.x), (ry + rect.y));
		}

		glEnd();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_BLEND);
	}

	// 检测矩形与圆的碰撞
	inline bool CheckR2C(const Rectangle& rect, const Circle& circle)
	{
		double x = circle.x - rect.x,
			y = circle.y - rect.y;
		Basic::Math::Matrix::RotateVec2_2D(x, y, rect.rotate * M_PI / 180, &x, &y);

		return (-(rect.width / 2 + circle.radius) <= x && x <= rect.width / 2 + circle.radius) &&
			(-(rect.height / 2 + circle.radius) <= y && y <= rect.height / 2 + circle.radius);
	}

	//检测四边形与圆的碰撞检测
	inline double DistanceBetweenTwoPoints(double x1, double y1, double x2, double y2)
	{
		return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
	}

	inline double DistanceFromPointToLine(double x, double y, double x1, double y1, double x2, double y2)
	{
		double a = y2 - y1;
		double b = x1 - x2;
		double c = x2 * y1 - x1 * y2;

		return abs(a * x + b * y + c) / sqrt(a * a + b * b);
	}

	inline bool VR2C(double x1, double y1, double x2, double y2,
		double x3, double y3, double x4, double y4,
		double cx, double cy, double r)
	{
		// 矩形中心坐标
		double rx = (x1 + x3) / 2;
		double ry = (y1 + y3) / 2;

		// 矩形上边中心
		double rux = (x1 + x2) / 2;
		double ruy = (y1 + y2) / 2;

		// 矩形右边中心
		double rrx = (x2 + x3) / 2;
		double rry = (y2 + y3) / 2;

		double w1 = DistanceBetweenTwoPoints(rx, ry, rrx, rry);
		double h1 = DistanceBetweenTwoPoints(rx, ry, rux, ruy);
		double w2 = DistanceFromPointToLine(cx, cy, rx, ry, rux, ruy);
		double h2 = DistanceFromPointToLine(cx, cy, rx, ry, rrx, rry);

		if (w2 > w1 + r)
			return false;
		if (h2 > h1 + r)
			return false;

		if (w2 <= w1)
			return true;
		if (h2 <= h1)
			return true;

		return (w2 - w1) * (w2 - w1) + (h2 - h1) * (h2 - h1) <= r * r;
	}

	inline bool VR2C(const double* verticles, const Circle& cir)
	{
		return VR2C(
			verticles[0],
			verticles[1],
			verticles[2],
			verticles[3],
			verticles[4],
			verticles[5],
			verticles[6],
			verticles[7],
			cir.x,
			cir.y,
			cir.radius
		);
	}

	//线与圆的碰撞检测
	inline bool L2C(double x1, double y1, double x2, double y2, double cx, double cy, double r)
	{
		double vx1 = cx - x1;
		double vy1 = cy - y1;
		double vx2 = x2 - x1;
		double vy2 = y2 - y1;

		// len = v2.length()
		double len = sqrt(vx2 * vx2 + vy2 * vy2);

		// v2.normalize()
		vx2 /= len;
		vy2 /= len;

		// u = v1.dot(v2)
		// u is the vector projection length of vector v1 onto vector v2.
		double u = vx1 * vx2 + vy1 * vy2;

		// determine the nearest point on the lineseg
		double x0 = 0.0f;
		double y0 = 0.0f;
		if (u <= 0)
		{
			// p is on the left of p1, so p1 is the nearest point on lineseg
			x0 = x1;
			y0 = y1;
		}
		else if (u >= len)
		{
			// p is on the right of p2, so p2 is the nearest point on lineseg
			x0 = x2;
			y0 = y2;
		}
		else
		{
			// p0 = p1 + v2 * u
			// note that v2 is already normalized.
			x0 = x1 + vx2 * u;
			y0 = y1 + vy2 * u;
		}

		return (cx - x0) * (cx - x0) + (cy - y0) * (cy - y0) <= r * r;
	}

	inline bool L2C(const double* verticles, const Circle& cir) {
		return L2C(
			verticles[0],
			verticles[1],
			verticles[2],
			verticles[3],
			cir.x,
			cir.y,
			cir.radius
		);
	}
}