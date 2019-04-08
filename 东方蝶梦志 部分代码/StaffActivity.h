#pragma once
#include "Activity.h"
#include "Sprite2D.h"
#include "SecTaskList.h"
#include "SecTweenData.h"
#include "MovieData.h"
#include "BaseBackground.h"
#include "BaseInputer.h"
namespace Activities
{
	class StaffActivity : public Basic::ActivityEngine::Activity
	{
	private:
		struct ImageUnit
		{
			float yOffset;	//相对的y坐标
			std::unique_ptr<Basic::Video::Renderer2D::Sprite2D> sprite;	//精灵
		};
		float m_height = 0;

		std::vector<ImageUnit> m_staffList;

		Basic::SecTweenData<float> m_alpha,m_ySub;
		
		Basic::SecTaskList m_tl;

		std::unique_ptr<MovieData::MovieData> m_movieData;


		std::unique_ptr<Background::BaseBackground> m_bk;
		Basic::SecTweenData<float> m_bkAlpha;
		int m_timer = 0;

		void changeToBackground(int bkID,bool = false);
	public:
		StaffActivity(std::unique_ptr<MovieData::MovieData>&& movieData);
		void OnEntry() override;

		void OnUpdate(float) override;
		void OnDraw() const override;
	};
}