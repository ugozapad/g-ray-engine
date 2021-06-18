#include "stdafx.h"
#include "staticobject.h"
#include "render/model.h"
#include "engine/input.h"
#include "engine/serializer.h"
#include "render/debug_render.h"

StaticObject::StaticObject()
{

}

StaticObject::~StaticObject()
{

}

void StaticObject::Update(float dt)
{
	inherited::Update(dt);
}

void StaticObject::Render()
{
	inherited::Render();

	DebugRender::Instance()->DrawSphere(m_Position);
}

void StaticObject::LoadVisual(const std::string& name)
{
	m_Model = g_resourceManager->LoadModel(name);
	m_Rotation.x = -90.0f;
}

void StaticObject::Load(Serializer* serializer)
{
	inherited::Load(serializer);

	std::string visualname;
	serializer->ReadString("VisualName", visualname);

	LoadVisual(visualname);
}

void StaticObject::Save(Serializer* serializer)
{
	inherited::Save(serializer);
}
