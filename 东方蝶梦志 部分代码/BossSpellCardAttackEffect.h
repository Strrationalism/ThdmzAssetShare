#pragma once
#include "GameObject.h"
#include "GLTexture.h"
#include "DataActor.h"
#include "TaskList.h"
#include <memory>
namespace Boss {
	class BossSpellCardAttackEffect :
		public GameObject
	{
	private:
		std::shared_ptr<Basic::Video::LowerRenderer::GLTexture> m_hint;

		double m_cnt = 0;
		double m_alpha = 0;

		Basic::DataActor m_dataActor;
		Basic::TaskList m_taskList;

		void drawTwoLines() const;
	public:
		BossSpellCardAttackEffect();
		~BossSpellCardAttackEffect();

		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}
