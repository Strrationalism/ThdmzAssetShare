#pragma once
#include "Bullet.h"
#include <memory>
namespace EnemyBullet {
	class Grid :
		public Bullet
	{
	private:
		std::optional<Dmsl::DmslUnit> m_dmsl;

		void callDmsl();
	public:

		void Create(int dmslUnit, int x, int y, const std::shared_ptr<const Dmsl::DmslVarying>& varyingIn);
		void Draw() const override;
		void Update() override;
		void Kill() override;
		void StopDmsl() override;
		Type WhatTypeAmI() const override;
		bool CollCheckWithCircle(const Collision::Circle&) const override;
		void EnableHDR(bool) override;
		void EnableTail(bool) override;

		void* operator new(size_t);
		void operator delete(void* ptr);
	};
}
