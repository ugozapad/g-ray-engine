#ifndef PHYSICS_WORLD_H
#define PHYSICS_WORLD_H


class PhysicsManager;

class PhysicsWorld
{
	friend class PhysicsManager;
public:
	void Step(float dt);

	//btDiscreteDynamicsWorld* GetWorld() { return m_physWorld; }

private:
	//btDiscreteDynamicsWorld* m_physWorld;
};

#endif // !PHYSICS_WORLD_H
