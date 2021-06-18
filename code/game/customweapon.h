#ifndef CUSTOMWEAPON_H
#define CUSTOMWEAPON_H

#include "engine/entity.h"

class CustomWeapon : public Entity
{
	typedef Entity inherited;
public:
	CustomWeapon();
	virtual ~CustomWeapon();

	virtual void Update(float dt);
	virtual void Render();

	virtual void MakeShot();

protected:
	int m_MaxAmmoAmount;
	int m_AmmoAmount;
	int m_PerShotDamage;
};

#endif // !CUSTOMWEAPON_H
