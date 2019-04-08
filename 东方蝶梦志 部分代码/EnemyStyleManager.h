#pragma once
#include <string>
#include <vector>
#include <memory>
#include "ImageCacheManager.h"
namespace Enemy {
	class EnemyStyleManager final
	{
	public:
		struct EnemyStyle {
			int gameFrame_animationFrame;
			std::string textureName;
			double collRadius;	//碰撞体半径
			bool xFilpEnabled;
			struct {	//被杀死时的粒子特效
				double r, g, b, speed;
				int num;
			}particleWhenKilled;
		};
	private:
		static std::array<EnemyStyle,25> m_styles;
	public:

		static void Init();

		inline const static EnemyStyle& GetStyle(int n) { return m_styles.at(n - 1); }
	};
}