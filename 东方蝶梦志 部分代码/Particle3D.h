#pragma once
#include <functional>
#include <vector>
#include <memory>
#include "Dmsl.h"
#include "GLTexture.h"
//#include <mutex>
namespace Basic {
	namespace Video {
		namespace Renderer3D {
			class Particle3D final
			{
			private:
				//static Dmsl::DmslVirtualMachine m_dvm;	//Dmsl虚拟机
				//static int m_dvm_attr_speed, m_dvm_attr_alpha, m_dvm_attr_angle, m_dvm_attr_r, m_dvm_attr_g, m_dvm_attr_b,
					//m_dvm_attr_x, m_dvm_attr_y, m_dvm_attr_size;


				static std::shared_ptr<LowerRenderer::GLTexture> m_texture;

			public:
				struct Dot3D {
					double speed = 0, alpha = 1, angle = 0, r = 1, g = 1, b = 1, size = 30;
					double self_angle = 0;
					std::function<void(Dot3D&, Particle3D&)> program;
					double userdata[4] = { 0 };
					double x, y;
					int style;
					GLenum blend_dst = GL_ONE_MINUS_SRC_ALPHA;
				};
			private:
				//std::mutex m_lock;
				std::vector<Dot3D> m_dot;	//粒子集
			public:
				Particle3D();
				static void InitSystem();

				void AddDot(int style, std::function<void(Dot3D&)> initer, std::function<void(Dot3D&, Particle3D&)> program , double x = 0, double y = 0, int num = 1);
				inline bool Empty() {
					return m_dot.empty();
				};

				inline auto Size() {
					return m_dot.size();
				}

				inline void Clear() {
					m_dot.clear();
				}

				//绘制前需要使用固定管线
				void Draw() const;
				void Update();
			};

		}
	}
}