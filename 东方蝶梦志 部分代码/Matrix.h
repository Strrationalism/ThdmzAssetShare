#pragma once
#include <cmath>
namespace Basic {
	namespace Math {
		namespace Matrix {
			inline void RotateVec2_2D(double x, double y, double angle, double* resultX, double* resultY) {
				//cos(a) sin(a)
				//-sin(a) cos(a)
				*resultX = x * cos(angle) - y * sin(angle);
				*resultY = x * sin(angle) + y * cos(angle);
			}
		}
	}
}