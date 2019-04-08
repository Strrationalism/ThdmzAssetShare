#pragma once
#include "Boss.h"

namespace Boss
{
	class BossWithWBEffectSpellCard : public Boss
	{
	private:
		GameObject* m_bk = nullptr;

		std::optional<Stage5DoremiBook> m_stage5DoremiBook;


	public:
		using Boss::Boss;
		void WBBGWasKilled();

		void SetWB(const double);

		void Update() override;
		void Draw() const override;

		inline void AttachStage5DoremiBook()
		{
			m_stage5DoremiBook.emplace();
		}

		void SpellCardOver() override;
	};
}