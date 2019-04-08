#pragma once
#include "Sp2DObject.h"
#include <memory>

namespace Conversation {
	class TextWindow :
		public Sp2DObject
	{
	private:
		double m_alpha = 0;
		double m_y = 400;
	public:
		TextWindow();
		void Update() override;
		void Kill() override;
	};
}