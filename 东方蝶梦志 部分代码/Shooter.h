#pragma once
#include <functional>
namespace Shooter {
	struct Shooter {
		int ways = 1, layer = 0, minLiveOutScreen = 0;
		bool spy = false;
		double
			angle = 0,
			vecangle = 0,
			range = 360,
			radius = 0;
		void Execute(std::function<void(double angle, double offset_x, double offset_y)>, double org_x, double org_y);
	};
}