#pragma once

namespace Basic::Menu
{
	class MenuItem
	{
	public:
		MenuItem() = default;
		MenuItem(const MenuItem&) = delete;

		virtual ~MenuItem() {}

		virtual void OnInitSelect(int offset) {}
		virtual void OnSelectChanged(int offset) {}
		virtual void OnEntry(int offset) {}
		virtual void Update(float deltaTime) {}
		virtual void Draw() const {}

		virtual bool IsTopDraw() const
		{
			return false;
		}
	};
}