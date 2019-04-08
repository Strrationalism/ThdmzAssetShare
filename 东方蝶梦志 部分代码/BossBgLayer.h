#pragma once
#include "GameObject.h"
#include "GLTexture.h"
#include <string>
#include <memory>
#include "DataActor.h"
#include "Dmsl.h"

namespace Boss {
	class BossBgLayer :
		public GameObject
	{
	private:
		static std::unique_ptr<Dmsl::DmslVirtualMachine> m_dvm;
		static struct Loc {
			int locX, locY, locAngle, locAlpha, locR, locG, locB, locXZoom, locYZoom;
		}m_loc;

		Dmsl::DmslUnit m_dmsl;

		Basic::Video::LowerRenderer::GLTexture m_texture;
		int m_w, m_h;

		int m_blendMode;	//混合模式

		double m_alpha2 = 0;	//这个Alpha由程序控制

		Basic::DataActor m_dataActor;

		double m_x, m_y, m_angle, m_alpha, m_xZoom, m_yZoom, m_r, m_g, m_b;

		void downloadData();	//从Dmsl获取数据
	public:
		static void LoadScript();
		~BossBgLayer();
		BossBgLayer(int dmslUnit, const std::string& texture, int blendMode);
		void Show();
		void Hide();

		void Update() override;
		void Draw() const override;
		void Kill() override;
	};
}