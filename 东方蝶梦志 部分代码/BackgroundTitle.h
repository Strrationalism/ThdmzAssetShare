#pragma once
#include <memory>
#include <functional>
#include "GLTexture.h"
#include "SecTaskList.h"
#include "SecTweenData.h"
#include "Sprite2D.h"
#include "ImageCacheManager.h"
#include "Shaders2.h"
#include "Shaders.h"
#include "Window.h"

namespace Activities::Title
{
	class BackgroundTitle
	{
	private:

		using PTex = std::shared_ptr<Basic::Video::LowerRenderer::GLTexture>;

		void drawFadingBg(
			float leftUV,
			float topUV,
			float rightUV,
			float bottomUV,
			float alpha
		) const;

		void drawFullBgUsingGLColor(
			float leftUV,
			float topUV,
			float rightUV,
			float bottomUV
		) const;


		Basic::SecTaskList m_secTasks;


		void reset();

		float m_time = 0;

		//BK Clouds
		PTex m_cloudTexture;
		Basic::SecTweenData<float> m_bkCloudAlpha;

		//Star Sky Images
		PTex m_noBluredStarSky;
		PTex m_bluredStarSky1, m_bluredStarSky2;
		Basic::SecTweenData<float> m_starSkyAlpha;

		//Grid Texture
		PTex m_gridTex;

		//Star Wind Texture
		PTex m_starWind;

		//SPEC Blend Layer
		void drawSpecBlendLayer(const std::function<void()>& drawcallsInYSJD) const;
		PTex m_mainScreen, m_specScreen;

	public:
		BackgroundTitle(const BackgroundTitle&) = delete;
		const BackgroundTitle& operator = (const BackgroundTitle&) = delete;

		BackgroundTitle();
		void Update(float t);
		
		template<typename T,typename U>
		inline void Draw(T&& drawcallsInYSJD, U&& drawcalls) const
		{
			int w, h;
			SDL_GetWindowSize(Basic::Video::Window::GetWindow(), &w, &h);

			{
				m_cloudTexture->Bind();
				Basic::Shaders::TitleClouds(m_time / 20, m_bkCloudAlpha);
				drawFadingBg(0, 0, 1, 1, m_bkCloudAlpha);
				glUseProgram(0);
			}

			{
				constexpr auto height = 0.75f;
				constexpr auto light = 0.5;


				glBlendFunc(GL_SRC_ALPHA, GL_ONE);



				m_bluredStarSky1->Bind();
				{
					const auto offset = m_time / 80;
					drawFadingBg(0, offset, 4, offset + 4 * height, light * m_starSkyAlpha + m_starSkyAlpha * 0.5 * (0.5*sin(m_time / 2) + 0.5));
				}

				{
					const auto offset = -m_time / 40;
					drawFadingBg(0, offset + 2 * height, 2, offset, light * m_starSkyAlpha + m_starSkyAlpha * 0.5 * (0.5 * sin(m_time / 2 + M_PI) + 0.5));
				}

				{
					const auto offset = m_time / 30;
					m_bluredStarSky2->Bind();
					drawFadingBg(0, offset, 1.5, 1.5*height + offset, light * m_starSkyAlpha + m_starSkyAlpha * 0.5 * sin(m_time));
				}

				{
					const auto offset = -m_time / 20;
					drawFadingBg(0, height + offset, 1, offset, light * m_starSkyAlpha + m_starSkyAlpha * 0.5 * sin(m_time + M_PI));
				}

				m_noBluredStarSky->Bind();
				{
					const auto offset = m_time / 80;
					drawFadingBg(0, offset, 1, height + offset, m_bkCloudAlpha);
				}
				glUseProgram(0);


			}
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			m_gridTex->Bind();
			drawFadingBg(0, 0, 1, 1, m_bkCloudAlpha * 0.2f);

			m_starWind->Bind();
			glColor4f(1, 1, 1, m_bkCloudAlpha);
			drawFullBgUsingGLColor(0, 0, 1, 0.75f);

			drawcalls();

			m_mainScreen->Bind();
			glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0);

			{
				glClear(GL_COLOR_BUFFER_BIT);
				drawSpecBlendLayer(drawcallsInYSJD);

				m_specScreen->Bind();
				glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 0, 0, w, h, 0);
			}

			{
				glClear(GL_COLOR_BUFFER_BIT);
				m_mainScreen->Bind(0);
				m_specScreen->Bind(1);
				Basic::Shaders::PSBlendYSJD();
				glColor4f(1, 1, 1, 1);
				drawFullBgUsingGLColor(0, 1, 1, 0);
				glActiveTexture(GL_TEXTURE0);
				glUseProgram(0);
			}

		}

		void FadeInNow();

		void FadeToBlackScreen();

		inline auto& GetStarSkyAlpha()
		{
			return m_starSkyAlpha;
		}
	};
}
