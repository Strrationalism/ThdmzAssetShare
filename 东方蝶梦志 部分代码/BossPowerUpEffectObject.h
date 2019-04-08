#include "GameObject.h"

namespace Boss
{
	class BossPowerUpEffectObject :
		public GameObject
	{
	private:
		int m_effectPowerUp = 160;
		double m_x, m_y;
		bool m_followPlayer;
	public:
		BossPowerUpEffectObject();	//¸úËæ×Ô»ú
		BossPowerUpEffectObject(double x,double y);

		void Draw() const override;
		void Update() override;
		void Kill() override;
	};
}