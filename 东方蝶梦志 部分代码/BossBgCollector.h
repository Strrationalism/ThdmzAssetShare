#pragma once
#include <string>
#include <vector>
#include <memory>
#include "GameObject.h"
#include "Dmsl.h"
namespace Boss {
	class BossBgCollector
	{
	private:
		std::vector<GameObject*> m_gos;
		bool m_active = false;
	public:
		~BossBgCollector();
		void LoadBossBG(const std::string& layerDefinedCSV);
		void Active();
		void Show();
		void Hide();
		void Kill();
	};
}
