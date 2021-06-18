#ifndef WEAPONRAIL_H
#define WEAPONRAIL_H

#include "customweapon.h"

class WeaponRail : public CustomWeapon
{
	typedef CustomWeapon inherited;
public:
	WeaponRail();
	~WeaponRail();

	void Update(float dt);
	void Render();
	void MakeShot();

};

#endif // !WEAPONRAIL_H
