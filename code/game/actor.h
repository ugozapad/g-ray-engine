#ifndef ACTOR_H
#define ACTOR_H

#include "engine/entity.h"
#include "customweapon.h"

class Camera;

class Actor : public Entity
{
	//RTTR_ENABLE(Actor, Entity)
	typedef Entity inherited;
public:
	Camera* m_camera;
	//CustomWeapon* m_rail;
public:
	Actor();
	~Actor();

	void Update(float dt);
	void Render();
};

#endif // !ACTOR_H
