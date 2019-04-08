#pragma once
#include <vector>
#include "GameObject.h"
#include <memory>
#include <stack>

class GameObjectManager final
{
private:

	//std::mutex m_insertLock;
//protected:
	std::shared_ptr<std::stack<std::unique_ptr<GameObject>>> m_ready;
	std::shared_ptr<std::vector<std::unique_ptr<GameObject>>> m_gos;
public:
	GameObjectManager();
	~GameObjectManager();
	void Draw() const;
	void Update();	//更新所有游戏物体

	void KillAll();	//杀死已存在的游戏物体
	void ForceKillAll();	//强制杀死所有已存在的游戏物体
	void Insert(GameObject* p);

	void Prep(size_t count);

	std::vector<std::unique_ptr<GameObject>>::iterator begin();

	std::vector<std::unique_ptr<GameObject>>::iterator end();

	size_t Size() const;

	GameObject& operator [](int n);
};
