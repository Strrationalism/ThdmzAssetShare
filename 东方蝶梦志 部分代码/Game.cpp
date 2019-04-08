#include "Precompiled.h"
#include "Game.h"
#include "EnemyStyleManager.h"
#include "BulletStyleManager.h"
#include "BossHPDisplay.h"
#include "Bullet.h"
#include "BossMahoujin.h"
#include "BossBgLayer.h"
#include "Shaders.h"
#include "Window.h"
#include "Shaders.h"
#include "ConversationLogic.h"
#include "ImageCacheManager.h"
#include "StageTitle.h"
#include "GameRebuilder.h"

void Game::loadStageTaskFromStageData()
{
	m_runningStageTask.Clear();
	for (auto p : m_stageData->stageTasks) {
		if(p.frames > 0)
			this->m_runningStageTask.AddTask(p.frames,p.task);
		else
			this->m_runningStageTask.AddTaskAfterLast(-p.frames, p.task);
	}
}

static std::optional<Basic::Video::LowerRenderer::GLTexture> backendFB;
void Game::Init()
{
	SDL_GL_GetDrawableSize(Basic::Video::Window::GetWindow(), &m_screenWidth, &m_screenHeight);

	if (Background::BaseBackground::UsingBeautifulBG()) {
		backendFB.emplace();
	}

	m_ui.emplace();

	
	m_mainTimer = 0;

	m_playerGrazeCollDisplay.GetSprite().UseImage("PlayerCollision", 0);
	m_playerDeathCollDisplay.GetSprite().UseImage("PlayerCollision", 1);
	m_playerGrazeCollDisplay.GetSprite().SetPos(10000, 10000);
	m_playerDeathCollDisplay.GetSprite().SetPos(10000, 10000);


	m_bossPosDisplay.UseImage("GameUI/BossHPPoint",1);
	m_bossPosDisplay.SetSize(1280, 24);

	glUseProgram(0);
	glBindTexture(GL_TEXTURE_2D, 0);

}

void Game::Quit()
{
	backendFB.reset();
}


void Game::Reset()
{
	m_parBottom.Clear();
	m_parTop.Clear();

	m_warningLineSystem.Clear();

	m_bossPosY = -9999;
	m_bossPosYAlpha = 0;
	m_score.ResetWithGameStage();
	m_drawStageBackground = true;
	m_player.ForceKill();
	m_player.Birth();
	m_player.SetMoveSpeedMul(1.0);
	m_playerBulletManager.ForceKillAll();
	m_enemyManager.ForceKillAll();
	m_enemyManager.Reset();
	m_enemyAnimation.Clear();
	m_enemyBulletManager.Reset();
	m_inManager.ForceKillAll();
	m_taskList.Clear();
	m_dataActor.Clear();
	m_miscManager.ForceKillAll();
	m_miscManagerBottom.ForceKillAll();
	m_runningStageTask.Clear();
	if (m_stageData) {
		loadStageTaskFromStageData();
		if (m_stageData->background)
			m_stageData->background->Reset();
	}
	m_items.Clear();
	m_items.SetEnabled(true);
	m_ui->Reset();
	//m_score.ResetWithGameStage();
	m_afterEffects.clear();
	//AddAfterEffect(new TestAfterEffect);
	//m_player.PowerUp(4);
	m_player.GetChar()->Reset();
	//m_player.SetPower(3);	//为了润滑和舒服，在发布时要去掉这行代码
	m_mainTimer = 0;

	//AddAfterEffect(new TestAfterEffect);
	srand(m_inputer->GetRandSeed());
	Log2("SRand:", m_inputer->GetRandSeed());

	//m_warningLineSystem.CreateLineCrossScreen(0, 0, 30, 1, 1, 1,500);
}


void Game::SetMirrorMode(bool b) {
	if (b) {
		m_mirror = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	}
	else {
		m_mirror.reset();
	}
}

bool Game::GetMirrorMode() const
{
	return m_mirror.has_value();
}

bool Game::IsMirrorMode() const
{
	return m_mirror.has_value();
}

void Game::Draw()
{
	//auto begin = SDL_GetTicks();

	//glClearColor(0, 0, 0, 1);
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(m_drawStageBackground) m_stageData->background->Draw();
	else
	{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	
	if (m_player.GetSheild().has_value())
		m_player.GetSheild()->OnDraw();


	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
	m_miscManagerBottom.Draw();
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();

	m_parBottom.Draw();

	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
	m_items.Draw();
	m_playerBulletManager.Draw();
	m_player.Draw();
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();

	if (m_bossPosY != -9999) {
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
		m_bossPosDisplay.Draw();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	m_enemyAnimation.Draw();

	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
	m_enemyManager.Draw();
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();

	m_warningLineSystem.Draw();

	m_enemyBulletManager.Draw();

	m_inManager.Draw();


	m_parTop.Draw();

	m_playerGrazeCollDisplay.GetSprite().Draw();	//这种绘制方法可以避免指针解引用
	m_playerDeathCollDisplay.GetSprite().Draw();



	if (!m_afterEffects.empty() && Background::BaseBackground::UsingBeautifulBG()) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, *backendFB);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//First Backend
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_screenWidth, m_screenHeight, 0);

		for (size_t i = 0; i < m_afterEffects.size(); ++i) {
			glClearColor(0, 0, 0, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_afterEffects[i]->Draw();
			if (i < m_afterEffects.size() - 1)
				glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_screenWidth, m_screenHeight, 0);
		}

	}

	//Mirror 模式
	if (m_mirror.has_value()) {
		double px, py;
		GetPlayer().GetPos(px, py);
		Basic::Shaders::MirrorModeWBEffect(px, py);
		glBindTexture(GL_TEXTURE_2D, **m_mirror);
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, m_screenWidth, m_screenHeight, 0);
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS); {
			glTexCoord2d(1, 1);
			glVertex2d(-WIDTH/2, -HEIGHT/2);
			glTexCoord2d(1, 0);
			glVertex2d(-WIDTH/2, HEIGHT/2);
			glTexCoord2d(0, 0);
			glVertex2d(WIDTH/2, HEIGHT/2);
			glTexCoord2d(0, 1);
			glVertex2d(WIDTH/2, -HEIGHT/2);
		}
		glEnd();
		glUseProgram(0);
	}

	Basic::Video::Renderer2D::Sprite2D::ClearFastBuffer();
	m_miscManager.Draw();
	Basic::Video::Renderer2D::Sprite2D::DrawFastBuffer();

	m_ui->Draw();



	//暂时取消这个边框
	/*for (auto& p : m_gameShell)
		p->Draw();*/
}

void Game::Update()
{

	if (m_bossPosY != -9999) {
		m_bossPosDisplay.SetPos(0, m_bossPosY);
		m_bossPosDisplay.SetAlpha(m_bossPosYAlpha * 0.5);
		m_bossPosDisplay.UpdateVertex();
	}


	static Basic::Input::Message e;
	while (m_inputer->PopMessage(e)) {
		m_player.OnEvent(e);
	}

	m_enemyAnimation.Update();

	m_stageData->background->Update();
	m_dataActor.Update();
	m_enemyManager.Update();
	m_player.Update();
	m_playerBulletManager.Update();
	m_enemyBulletManager.Update();
	m_inManager.Update();
	m_parTop.Update();
	m_parBottom.Update();
	m_items.Update();
	m_taskList.Update();
	m_runningStageTask.Update();
	m_miscManager.Update();
	m_miscManagerBottom.Update();
	m_mainTimer++;

	m_warningLineSystem.Update();

	m_ui->Update();

	m_afterEffects.erase(std::remove_if(m_afterEffects.begin(), m_afterEffects.end(), [](auto& p) {
		if (!p) return true;
		return p->Killed();
	}),m_afterEffects.end());
	for (auto& p : m_afterEffects)
		p->Update();

	for (auto& t : m_importantTask)
		t();
	m_importantTask.clear();
}

void Game::SetInputer(Inputer::BaseInputer& i)
{
	m_inputer = &i;
}

void Game::ReleaseKeys()
{
	Basic::Input::Message msg;

	msg.type = Basic::Input::Message::Type::VecMotion;
	msg.vec.x = 0;
	msg.vec.y = 0;

	m_inputer->PushMessage(msg);

	msg.type = Basic::Input::Message::Type::KeyUp;
	msg.key = Basic::Input::Key::Slow;
	m_inputer->PushMessage(msg);

	msg.key = Basic::Input::Key::Shoot;
	m_inputer->PushMessage(msg);
}

void Game::SetStageData(const std::shared_ptr<StageData>& s)
{
	m_stageData = s;
	m_enemyManager.SetStageEnemyData(s);

	m_stageData->background->Init();
	m_stageData->background->Reset();

	m_enemyBulletManager.SetLayerNum(s->bulletLayerNum);
	EnemyBullet::Bullet::SetLevel(s->level);

	loadStageTaskFromStageData();

	if (m_stageData->stageBgm != -1) {
		m_taskList.AddTask([this]() {
			Basic::Sound::BgmPlayer::Play(m_stageData->stageBgm);
		}, 1);
	}
	else {
		m_taskList.AddTask([this]() {
			Basic::Sound::BgmPlayer::Stop();
		}, 1);
	}

	for (auto p : s->bulletNoKillLayerWhenPlayerDied) {
		m_enemyBulletManager.SetNoKillLayerByPlayerDied(p);
	}
}

void Game::LoadFromInitData(GameInitData & d)
{
	m_player.SetPower(d.power);
	m_score.SetScore(d.score);
	m_score.SetDream(d.dream);
	m_score.SetLife(d.life);
	m_score.SetLifeKagera(d.lifeKagera);
	m_player.SetPos(d.playerX, d.playerY);
	m_score.SetGraze((unsigned)d.graze);
}

void Game::WriteToInitData(GameInitData & d)
{
	/* Player数据 */
	d.power = m_player.GetPower();	//火力

	/* 分数 */
	d.score = m_score.GetScore();

	/* 梦秤 */
	d.dream = m_score.GetDream();

	/* 自机坐标 */
	m_player.GetPos(d.playerX, d.playerY);

	d.life = m_score.GetLife();
	d.lifeKagera = m_score.GetLifeKagera();

	d.graze = (uint32_t)m_score.GetGraze();
}

std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> Game::Screenshot() const
{
	auto ret = std::make_shared<Basic::Video::LowerRenderer::GLTexture>();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Game::GetGame().Draw();
	glBindTexture(GL_TEXTURE_2D, *ret);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, m_screenWidth, m_screenHeight, 0);

	return ret;
}


static std::optional<Game> theGame;
Game & Game::GetGame() 
{
	if (!theGame.has_value())
		Crash(L"未能初始化。");
	return *theGame;
}

void RebuildGame()
{
	theGame.emplace();
	theGame->Init();
}

void ClearGame()
{
	theGame.reset();
}

void GameInitData::WriteToStream(std::ostream & out) const
{
	out.write((char*)&power, sizeof(power));
	out.write((char*)&score, sizeof(score));
	out.write((char*)&dream, sizeof(dream));
	out.write((char*)&life, sizeof(life));
	out.write((char*)&lifeKagera, sizeof(lifeKagera));
	out.write((char*)&playerX, sizeof(playerX));
	out.write((char*)&playerY, sizeof(playerY));
	out.write((char*)&graze, sizeof(graze));
}

void GameInitData::ReadFromStream(std::istream & in)
{
	in.read((char*)&power, sizeof(power));
	in.read((char*)&score, sizeof(score));
	in.read((char*)&dream, sizeof(dream));
	in.read((char*)&life, sizeof(life));
	in.read((char*)&lifeKagera, sizeof(lifeKagera));
	in.read((char*)&playerX, sizeof(playerX));
	in.read((char*)&playerY, sizeof(playerY));
	in.read((char*)&graze, sizeof(graze));
}
