#include "stdafx.h"
#include "physics_manager.h"
#include "physics_world.h"

PhysicsManager g_physicsManager;

void PhysicsManager::Init()
{
	//m_collisionConfig = New(btDefaultCollisionConfiguration);
	//m_dispatcher = New(btCollisionDispatcher, m_collisionConfig);
	//m_pairCache = New(btDbvtBroadphase);
	//m_solver = New(btSequentialImpulseConstraintSolver);
}

void PhysicsManager::Shutdown()
{
	//if (m_solver)
	//	mem_delete(m_solver);

	//if (m_pairCache)
	//	mem_delete(m_pairCache);

	//if (m_dispatcher)
	//	mem_delete(m_dispatcher);

	//if (m_collisionConfig)
	//	mem_delete(m_collisionConfig);
}

PhysicsWorld* PhysicsManager::CreatePhysicsWorld()
{
	PhysicsWorld* physicsWorld = New(PhysicsWorld);
	//physicsWorld->m_physWorld = New(btDiscreteDynamicsWorld, m_dispatcher, m_pairCache, m_solver, m_collisionConfig);
	return physicsWorld;
}

void PhysicsManager::DeletePhysicsWorld(PhysicsWorld* world)
{
	ASSERT(world);
	//ASSERT(world->m_physWorld);

	//mem_delete(world->m_physWorld);
	mem_delete(world);
}
