#include "stdafx.h"
#include "ilevel.h"
#include "engine/entity.h"
#include "render/model.h"
#include "filesystem/filesystem.h"
#include "engine/serializer.h"
#include "render/deferred_renderer.h"

ILevel* g_pILevel = nullptr;

void ILevel::Load(const char* filename)
{
}

void ILevel::Save(const char* filename)
{
}

void ILevel::Destroy()
{
	for (int i = 0; i < m_entities.size(); i++)
	{
		if (m_entities[i])
		{
			delete m_entities[i];
			m_entities[i] = NULL;
		}
	}	

	m_entities.clear();
}

void ILevel::Update(float dt)
{
	if (!m_isLoaded)
		return;

}

#include "engine/engine.h"
#include "engine/application.h"
#include "engine/camera.h"
#include "render/render.h"
#include "render/shadowrenderer.h"
#include "render/device.h"
#include "render/culling.h"

void ILevel::Render_Geom()
{
	if (!m_isLoaded)
		return;


}

void ILevel::Render_Skybox()
{
	if (!m_isLoaded)
		return;
}

void ILevel::Render_Shadows()
{

}

Entity* ILevel::CreateEntity(const char* classname)
{
	return nullptr;
}
