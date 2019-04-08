#pragma once

namespace AfterEffect {
	class BaseAfterEffect {
	public:
		virtual ~BaseAfterEffect() {}
		virtual void Draw() const = 0;	//此时绘制时，已经绑定了纹理到TEXTURE0
		virtual void Update() = 0;
		virtual bool Killed() = 0;
	};
}
