#pragma once
#include "ScoreDisplayer.h"
#include "DreamSystem.h"
#include "GrazeDisplayer.h"
#include "NonFullHeart.h"
#include "FullHearts.h"
namespace UI {
	class UIManager
	{
	private:
		ScoreDisplayer m_scoreDisplayer;
		GrazeDisplayer m_grazeDisplayer;
		DreamSystem m_dreamSystem;
		NonFullHeart m_nonFullHeart;
		FullHearts m_fullHearts;
	public:
		inline void Reset() {
			m_scoreDisplayer.Reset();
			m_dreamSystem.Reset();
			m_grazeDisplayer.Reset();
		}
		inline void Draw() const {
			m_scoreDisplayer.Draw();
			m_grazeDisplayer.Draw();
			m_dreamSystem.Draw();
			m_nonFullHeart.Draw();
			m_fullHearts.Draw();
		}
		inline void Update() {
			m_scoreDisplayer.Update();
			m_dreamSystem.Update();
			m_grazeDisplayer.Update();
			m_nonFullHeart.Update();
			m_fullHearts.Update();
		}

		inline auto& GetGrazeDisplay() {
			return m_grazeDisplayer;
		}

		inline auto& GetScoreDisplay() {
			return m_scoreDisplayer;
		}

		inline auto& GetDreamSystem() {
			return m_dreamSystem;
		}

		inline auto& GetNonFullHeart() {
			return m_nonFullHeart;
		}

		inline auto& GetFullHearts() {
			return m_fullHearts;
		}
	};
}