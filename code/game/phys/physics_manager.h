#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

//#include "LinearMath/btVector3.h"
//#include "btBulletDynamicsCommon.h"

class PhysicsWorld;

class PhysicsManager
{
public:
	void Init();
	void Shutdown();

	PhysicsWorld* CreatePhysicsWorld();
	void DeletePhysicsWorld(PhysicsWorld* world);

private:
	//btDefaultCollisionConfiguration* m_collisionConfig;
	//btCollisionDispatcher* m_dispatcher;
	//btBroadphaseInterface* m_pairCache;
	//btSequentialImpulseConstraintSolver* m_solver;

};

extern PhysicsManager g_physicsManager;

#endif // !PHYSICS_MANAGER_H
