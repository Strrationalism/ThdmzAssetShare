#include "Precompiled.h"
#include "BulletStyleManager.h"
#include "ImageCacheManager.h"
#include "CSVReader.h"

EnemyBullet::BulletStyleManager::BulletStyleManager()
{
	Basic::CSVReader csv;

	csv.Load("BulletStyles.csv");

	while (!csv.IsLastLine()) {
		auto id = csv.PopInt();
		auto& style = m_styles[id];
		style.textureName = std::to_string(id);

		style.subStyleNum = csv.PopInt();
		style.initSize = csv.PopDouble();
		style.animated = csv.PopInt() > 0;
		style.collArg1 = csv.PopDouble();

		csv.NextLine();
	}
}

EnemyBullet::BulletStyleManager & EnemyBullet::BulletStyleManager::GetMgr()
{
	static BulletStyleManager mgr;
	return mgr;
}