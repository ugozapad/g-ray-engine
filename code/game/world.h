#ifndef WORLD_H
#define WORLD_H

#include "engine/ilevel.h"
#include "render/model.h"

class Entity;
class PhysicsWorld;

class World : public ILevel
{
public:
	// Load world.
	virtual void Load(const char* filename);

	// Save world.
	virtual void Save(const char* filename);

	// Destroy world.
	virtual void Destroy();

	// Update at single frame.
	virtual void Update(float dt);

	// Render at single frame.
	virtual void Render_Geom();
	virtual void Render_Skybox();
	virtual void Render_Light();
	virtual void Render_Shadows();

	Entity** GetEntitiesBegin() { return m_entities.data(); }
	Entity** GetEntitiesEnd() { return m_entities.data() + m_entities.size(); }

	bool IsLoaded() { return m_isLoaded; }

	Entity* CreateEntity(const char* classname);

public:
	Entity* m_currentViewEntity;

private:
	std::vector<Entity*> m_entities;
	std::shared_ptr<ModelBase> m_skybox;
	PhysicsWorld* m_physicsWorld;
};

#endif // !WORLD_H
