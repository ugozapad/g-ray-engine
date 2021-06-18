#include "stdafx.h"
#include "world.h"
#include "engine/entity.h"
#include "actor.h"
#include "staticobject.h"
#include "render/model.h"
#include "filesystem/filesystem.h"
#include "engine/serializer.h"
#include "objectfactory.h"
#include "actor.h"
#include "phys/physics_world.h"
#include "phys/physics_manager.h"
#include "render/deferred_renderer.h"

void World::Load(const char* filename)
{
	m_levelName = strdup(IFileSystem::Instance()->GetFileName(filename).c_str());
	Msg("loading level %s", m_levelName);

	// init physics.
	g_physicsManager.Init();

	// create physics world
	m_physicsWorld = g_physicsManager.CreatePhysicsWorld();
	/*m_physicsWorld->GetWorld()->setGravity(btVector3(0, -10, 0));*/

	std::string skyboxname;
	Serializer serializer;
	serializer.Load(filename, this, skyboxname);

	if (skyboxname != "none")
		m_skybox = g_resourceManager->LoadModel(skyboxname);
	
	// Try to find local player.
	Actor* actor = dynamic_cast<Actor*>(m_entities[0]);
	if (!actor)
	{
		for (Entity** it = GetEntitiesBegin(); it != GetEntitiesEnd(); ++it)
			if (strcmp((*it)->m_ClassName, "Actor") == 0)
				actor = reinterpret_cast<Actor*>(*it);
	}

	ASSERT(actor);
	m_currentViewEntity = actor;

	m_isLoaded = true;
}

void World::Save(const char* filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenWriteStream(filename);

	Serializer serializer;
	serializer.BeginSave();

	for (int i = 0; i < m_entities.size(); i++)
		if (m_entities[i])
			serializer.SaveEntity(m_entities[i]);

	serializer.Save(stream);
}

void World::Destroy()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i])
		{
			delete m_entities[i];
			m_entities[i] = NULL;
		}
	}	
	
	// release physics world
	if (m_physicsWorld)
		g_physicsManager.DeletePhysicsWorld(m_physicsWorld);
	
	g_physicsManager.Shutdown();

	m_entities.clear();
	m_isLoaded = false;
}

void World::Update(float dt)
{
	if (!m_isLoaded)
		return;

	ASSERT(m_physicsWorld);
	m_physicsWorld->Step(dt);

	for (Entity** it = GetEntitiesBegin(); it != GetEntitiesEnd(); ++it)
	{
		Entity* entity = *it;
		if (entity)
			entity->Update(dt);
	}
}

#include "engine/engine.h"
#include "engine/application.h"
#include "engine/camera.h"
#include "render/render.h"
#include "render/shadowrenderer.h"
#include "render/device.h"
#include "render/culling.h"

void World::Render_Geom()
{
	if (!m_isLoaded)
		return;

	// get camera
	Camera* camera = reinterpret_cast<Actor*>(m_currentViewEntity)->m_camera;
	std::vector<Entity*> sortedEntites;

	// sort
	for (Entity** it = GetEntitiesBegin(); it != GetEntitiesEnd(); ++it)
		if (OcclusionCulling::Instance()->IsVisible(camera, (*it)->GetBoundedBox(), (*it)->m_Position))
			sortedEntites.push_back(*it);

	// pass 0 - geometry
	for (Entity** it = sortedEntites.data(); it != sortedEntites.data() + sortedEntites.size(); ++it)
		g_deferredRenderer.drawGeometry(camera, *it);
}

void World::Render_Skybox()
{
	if (!m_isLoaded)
		return;

	Camera* camera = reinterpret_cast<Actor*>(m_currentViewEntity)->m_camera;

	// render skybox
	if (m_skybox)
	{
		glm::mat4 transform = glm::mat4(1.0f);
		transform = glm::translate(transform, camera->GetPosition());
		transform = glm::rotate(transform, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

		RenderContext& ctx = g_render->GetRenderContext();
		ctx.proj = camera->GetProjectionMatrix();
		ctx.view = camera->GetViewMatrix();
		ctx.model = transform;
		g_render->SetRenderContext(ctx);

		m_skybox->RenderObjects(g_render->GetRenderContext());
	}

}

void World::Render_Shadows()
{

}

#include "filesystem/filesystem.h"

char asciitolower(char in)
{
	if (in <= 'Z' && in >= 'A')
		return in - ('Z' - 'z');
	return in;
}

Entity* World::CreateEntity(const char* classname)
{
	std::string filename = IFileSystem::Instance()->GetFileName(m_levelName);
	std::transform(filename.begin(), filename.end(), filename.begin(),
		[](unsigned char c) { return std::tolower(c); });

	std::string classnamestr = classname;
	std::transform(classnamestr.begin(), classnamestr.end(), classnamestr.begin(),
		[](unsigned char c) { return std::tolower(c); });

	char buffer[256];
	sprintf(buffer, "%s_%s_%i", filename.c_str(), classnamestr.c_str(), 0);

	Entity* entity = GetObjectFactory()->CreateObject(classname);
	entity->m_Name = strdup(buffer);

	m_entities.push_back(entity);

	return entity;
}
