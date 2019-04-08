#pragma once
#include "GLTexture.h"
#include "ImageCacheManager.h"
#include "GLCallList.h"
#include <memory>
#include <array>
namespace Basic {
	namespace Video {
		namespace Renderer2D {
			class NumberDisplay
			{
			private:
				std::shared_ptr<LowerRenderer::GLTexture> m_tex;

				std::array<LowerRenderer::GLCallList,10> drawNumber;

				int m_displayFormat = -1;
				std::string m_number;

				double m_alpha = 1, m_r = 1, m_g = 1, m_b =1;
				double m_zoom = 1;
				double m_x = 0, m_y = 0;
				double m_singleW = 0;

				bool m_leftTopPos = false;
			public:
				NumberDisplay(const std::string& styleTex);

				NumberDisplay(const NumberDisplay&) = delete;
				NumberDisplay(NumberDisplay&&) = delete;
				const NumberDisplay& operator = (NumberDisplay&&) = delete;
				const NumberDisplay& operator = (const NumberDisplay&) = delete;

				void Draw() const;

				inline void SetNumber(unsigned long long number) {
					m_number = std::to_string(number);
					for (auto& p : m_number)
						p -= '0';
				}
				inline void SetDisplayFormat(int show = -1) {	//显示共多少位数字，-1为忽略前置0
					m_displayFormat = show;
				}

				inline void SetAlpha(double alpha) {
					m_alpha = alpha;
				}
				inline void SetColorFliter(double r, double g, double b) {
					m_r = r;
					m_g = g;
					m_b = b;
				}
				inline void SetZoom(double zoom) {
					m_zoom = zoom;
				}

				inline void SetPos(double x, double y) {	//设置左上角
					m_x = x;
					m_y = y;
				}

				inline void LeftTopPos(bool b) {
					m_leftTopPos = b;
				}
			};
		}
	}
}