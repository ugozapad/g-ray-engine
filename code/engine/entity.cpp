#include "stdafx.h"
#include "entity.h"
#include "serializer.h"
#include "render/model.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

Entity::Entity()
{
	m_Parent = NULL;
	m_Model = NULL;

	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Scale    = glm::vec3(1.0f, 1.0f, 1.0f);

	m_XForm = glm::mat4(1.0f);

	m_EntityFlags = 0;
}

Entity::~Entity()
{

}

void Entity::Update(float dt)
{
	if (m_EntityFlags & Entity::EF_DISABLE)
		return;
}

#include "render/gfxcommon.h"
#include "render/render.h"

void Entity::Render()
{
	if (m_EntityFlags & Entity::EF_INVISIBLE)
		return;

	if (m_EntityFlags & Entity::EF_NOVISUAL)
		return;

	RenderContext& ctx = g_render->GetRenderContext();
	ctx.model = GetTranslation();
	g_render->SetRenderContext(ctx);

	if (m_Model)
		m_Model->RenderObjects(ctx);
}

void Entity::Load(Serializer* serializer)
{
	serializer->ReadVec3("Position", m_Position);
	serializer->ReadVec3("Rotation", m_Rotation);
	serializer->ReadVec3("Scale", m_Scale);
}

void Entity::Save(Serializer* serializer)
{
	serializer->WriteVec3("Position", m_Position);
	serializer->WriteVec3("Rotation", m_Rotation);
	serializer->WriteVec3("Scale", m_Scale);

	if (m_Model)
		serializer->WriteString("VisualName", m_Model->GetFileName());
}

glm::mat4 Entity::GetTranslation()
{
	glm::vec3 radiansRotaton = glm::vec3(glm::radians(m_Rotation.x), glm::radians(m_Rotation.y), glm::radians(m_Rotation.z));
	glm::mat4 rotation = glm::toMat4(glm::quat(radiansRotaton));
	glm::mat4 tranlation = glm::mat4(1.0f);

	if (GetParent())
		tranlation = GetParent()->GetTranslation() * glm::translate(tranlation, m_Position) * rotation * glm::scale(tranlation, m_Scale);
	else
		tranlation = glm::translate(tranlation, m_Position) * rotation * glm::scale(tranlation, m_Scale);

	return tranlation;
}
