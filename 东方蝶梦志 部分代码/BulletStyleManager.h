#pragma once
#pragma once
#include <map>
#include <vector>
#include "GLTexture.h"
#include <memory>
namespace EnemyBullet {
	class BulletStyleManager
	{
	public:
		struct BulletStyle {
			std::string textureName;
			bool animated;
			int subStyleNum;
			double initSize;
			double collArg1;
		};
	private:
		std::map<int,BulletStyle> m_styles;
		
	public:
		BulletStyleManager();

		static BulletStyleManager& GetMgr();

		size_t GetStylesSize() {
			return m_styles.size();
		}

		inline BulletStyle& GetStyle(int style) {
			return m_styles.at(style);
		}
	};
}