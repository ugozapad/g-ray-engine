#include "stdafx.h"
#include "weaponrail.h"

WeaponRail::WeaponRail()
{

}

WeaponRail::~WeaponRail()
{

}

void WeaponRail::Update(float dt)
{
	inherited::Update(dt);
}

void WeaponRail::Render()
{
	inherited::Render();
}

void WeaponRail::MakeShot()
{
	inherited::MakeShot();
	Msg("Pew!");
}

