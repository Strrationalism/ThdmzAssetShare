#pragma once

#include <memory>
namespace Background {
	class BaseBackground
	{
	private:
		static bool m_usingBeautifulBG;
	protected:

	public:
		virtual ~BaseBackground() {}
		virtual void Draw() const = 0;
		virtual void Update() = 0;

		virtual void UpdateWithoutGame();

		virtual void Init() = 0;

		virtual void Reset() = 0;

		virtual inline void SkipToBossMode() {}

		static std::unique_ptr<BaseBackground> CreateBackground(int n);
		static void SetUsingBeautifulBG(bool);
		static bool GetUsingBeautifulBG();
		static inline bool UsingBeautifulBG() {
			return m_usingBeautifulBG;
		}
	};
}