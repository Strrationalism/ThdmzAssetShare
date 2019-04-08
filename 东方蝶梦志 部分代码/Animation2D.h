#pragma once
#include "Sprite2D.h"
namespace Basic {
	namespace Video {
		namespace Renderer2D {
			class Animation2D final: public Basic::Video::Renderer2D::Sprite2D
			{
			private:
				int m_allFrame = 0, m_nowFrame = 0,m_begin = 0,m_timer = 0,m_screenFrame_frame = 0;
				Basic::Video::LowerRenderer::GLTexture::UVRect m_imageRect;
				void setSrc(int num);
			public:
				using Sprite2D::UseImage;
				//UseImage设置动画，ctx文件,每多少屏幕帧走一帧动画，开始的图像编号，走过的图像编号。
				void UseImage(const std::string& ctx,int screenFrame_frame, int begin, int num = -1);
				inline void UseText(){}	//不要使用UseText函数
				void Update();
				inline void Draw() const
				{
					Sprite2D::Draw();
				}

				inline auto GetNowFrame()
				{
					return m_nowFrame;
				}

				//绘制前依旧需要先调用Sprite2D::UseShader
			};
		}
	}
}