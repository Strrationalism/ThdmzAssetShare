#include "NuzPrecompiled.h"
#include "Init.h"

#include "SeManager.h"
#include "BGMPlayer.h"
#include "ConversationLogic.h"
#include "BasicInput.h"
#include "UnicodeSite.h"
#include "Shaders.h"
#include "Particle3D.h"
#include "BasicChar.h"
#include "Game.h"
#include "EnemyStyleManager.h"
#include "BulletStyleManager.h"
#include "EnemyStyleManager.h"
#include "Bullet.h"
#include "Boss.h"
#include "BossBgLayer.h"
#include "StageTitle.h"
void Basic::Init::SimpleInit()
{
	Basic::Sound::SeManager::Init();
	Conversation::ConversationWord::Init();
	Char::LoadAndSetConfig();
}

void Basic::Init::InitMustInMainThread()
{
	Basic::Shaders::Init();
	Basic::Input::Init();

	Enemy::EnemyStyleManager::Init();
	EnemyBullet::BulletStyleManager::GetMgr();
	EnemyBullet::Bullet::LoadScript();
	Boss::BossHPDisplay::Init();
	Boss::BossBgLayer::LoadScript();
	StageTask::StageTitle::StageTitleObj::Init();
	Conversation::ConversationStandImage::Init();

	Basic::Video::Renderer3D::Particle3D::InitSystem();
}

void Basic::Init::InitOpenGL()
{
	glEnable(GL_TEXTURE_2D);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glLineWidth(1.0);

	//设置摄像机
	glMatrixMode(GL_PROJECTION);
	glOrtho(-WIDTH / 2, WIDTH / 2, HEIGHT / 2, -HEIGHT / 2, 0, 100000);
	glMatrixMode(GL_MODELVIEW);
}

int Basic::Init::CacheTexture(Basic::TaskList& cacheTaskList, std::any& cache)
{
	using TexCacher = std::vector < std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>>;

	cache = TexCacher{};
	TexCacher& textureCache = std::any_cast<decltype(textureCache)>(cache);
	
	Basic::CSVReader texCacheList;

	// Warning！！纹理缓冲器需要区分高画质模式和低画质模式！！
	texCacheList.Load(Background::BaseBackground::UsingBeautifulBG() ? "Textures/TexList.csv" : "Textures/TexListLOW.csv");

	int timer = 2;
	while (!texCacheList.IsLastLine())
	{
		const std::string texName = texCacheList.PopString();
		cacheTaskList.AddTask([texName,&textureCache] {
			textureCache.emplace_back(Basic::Video::LowerRenderer::ImageCacheManager::LoadImageA(texName));
		}, timer++);
		texCacheList.NextLine();
	}

	return timer + 2;
}
