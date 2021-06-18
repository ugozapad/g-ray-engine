#include "stdafx.h"
#include "customweapon.h"
#include "actor.h"

CustomWeapon::CustomWeapon()
{
	m_MaxAmmoAmount = 0;
	m_AmmoAmount = 0;
	m_PerShotDamage = 0;
}

CustomWeapon::~CustomWeapon()
{

}

void CustomWeapon::Update(float dt)
{
	inherited::Update(dt);
}

void CustomWeapon::Render()
{
	inherited::Render();
}

void CustomWeapon::MakeShot()
{
	glm::vec3 shotDir;
}
