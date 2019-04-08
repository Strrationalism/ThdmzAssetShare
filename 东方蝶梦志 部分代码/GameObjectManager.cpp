#include "Precompiled.h"
#include "GameObjectManager.h"
#include <algorithm>
#include "Game.h"
using namespace std;


GameObjectManager::GameObjectManager()
{
	m_ready = std::make_shared<std::stack<std::unique_ptr<GameObject>>>();
	m_gos = std::make_shared<std::vector<std::unique_ptr<GameObject>>>();
}

GameObjectManager::~GameObjectManager()
{
	ForceKillAll();
}

void GameObjectManager::Draw() const
{
	static std::vector<GameObject*> drawTop;

	for (auto& p : *m_gos) {
		if (p->Visible()) {
			if (p->DrawOnTop())
				drawTop.push_back(p.get());
			else
				p->Draw();
		}
	}

	for (auto p : drawTop)
		p->Draw();

	drawTop.clear();
}

void GameObjectManager::Update()
{
	while (!m_ready->empty()) {
		m_gos->push_back(std::move(m_ready->top()));
		m_ready->pop();
	}

	for (auto& p : *m_gos)
		p->Update();

	m_gos->erase(
		remove_if(m_gos->begin(), m_gos->end(), [](auto& i) {
			return i->Died();
			}
		)
		,m_gos->end()
	);
}

void GameObjectManager::KillAll()
{
	if (m_gos->size() >= 1000) {
		//性能紧张
		ForceKillAll();
	}
	else {
		//性能不紧张
		while (!m_ready->empty()) {
			m_ready->pop();
		}
		//int size = m_gos.size();
		//#pragma omp parallel for
		for (auto& p : *m_gos) {
			if (!p->Died()) p->Kill();
		}
	}

}

void GameObjectManager::ForceKillAll()
{
	while (!m_ready->empty()) {
		m_ready->pop();
	}
	m_gos->clear();
}

//强制杀死所有已存在的游戏物体

void GameObjectManager::Insert(GameObject * p)	//添加游戏物体，此后为自动管理，不需要手动释放
{
	//std::lock_guard<std::mutex> g(m_insertLock);
	m_ready->push(std::unique_ptr<GameObject>(p));
}

void GameObjectManager::Prep(size_t count) {
	m_gos->reserve(count);
}

std::vector<std::unique_ptr<GameObject>>::iterator GameObjectManager::begin() {
	return m_gos->begin();
}

std::vector<std::unique_ptr<GameObject>>::iterator GameObjectManager::end() {
	return m_gos->end();
}

size_t GameObjectManager::Size() const {
	return m_gos->size();
}

GameObject & GameObjectManager::operator[](int n)
{
	return *m_gos->at(n);
}
