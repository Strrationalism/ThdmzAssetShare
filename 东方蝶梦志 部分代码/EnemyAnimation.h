#pragma once
#include <vector>

namespace Enemy {
	class EnemyAnimation
	{
	private:
		struct Animation
		{
			double angle;
			double frame;
			double r, g, b;
			double x, y;
		};

		std::vector<Animation> m_animation;

	public:
		void Push(double x,double y,double r, double g, double b);
		void Update();
		void Draw() const;	//警告！！需要单独的FastBuffer，使用前清空FastBuffer，使用后FastBuffer为空。

		inline void Clear()
		{
			m_animation.clear();
		}
	};
}
