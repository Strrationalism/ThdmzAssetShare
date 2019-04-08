#pragma once
#include <cmath>
#include "SDL_stdinc.h"
namespace Basic {
	namespace Math {
		/* 直线方程 */
		struct XFLine {
			double k, b;
		};

		/* 给出两个点求直线方程 */
		inline void GetLineXF(XFLine& xf, double x1, double y1, double x2, double y2) {
			if (x2 == x1) xf.k = 10000000000;
			else xf.k = float(y2 - y1) / (x2 - x1);
			xf.b = y1 - xf.k*x1;
		}

		/* 给一个点和一个角度制角度求直线方程 */
		inline void GetLineXF(XFLine& xf, double x, double y, double angle) {
			if (angle >= 89 && angle <= 91) xf.k = 10000000000;
			else xf.k = tan(angle*M_PI / 180);
			xf.b = y - xf.k*x;
		}

		/* 带入直线方程 */
		inline double GetY(const XFLine& xf, double x) {
			return xf.k*x + xf.b;
		}

		/* 根据Y求X */
		inline double GetX(const XFLine& xf, double y) {
			double k = xf.k;
			if (k == 0) k = 0.000000000001;
			return (y - xf.b) / k;
		}

		/* 求证点在两条直线之间 */
		inline bool PointIn2XFLines(const XFLine& xf1, const XFLine& xf2, double x, double y) {
			double y1 = GetY(xf1, x);
			double y2 = GetY(xf2, x);
			return (y >= y1 && y <= y2) || (y <= y1&&y >= y2);
		}
	}
}