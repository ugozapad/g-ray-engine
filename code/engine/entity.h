#ifndef ENTITY_H
#define ENTITY_H

#include "render/model.h"

class Serializer;

class Entity
{
public:
	Entity();
	virtual ~Entity();

	void SetParent(Entity* entity) { m_Parent = entity; }
	Entity* GetParent() { return m_Parent; }
	Entity* GetRoot() { return m_Parent ? m_Parent->GetRoot() : this; }

	virtual void Update(float dt);
	virtual void Render();

	virtual void Load(Serializer* serializer);
	virtual void Save(Serializer* serializer);

	glm::mat4 GetTranslation();

	AABB* GetBoundedBox() { return m_Model ? m_Model->GetBoundedBox() : NULL; }

public:
	enum EntityFlags
	{
		EF_NONE      = (1<<0),
		EF_INVISIBLE = (1<<1),
		EF_NOVISUAL  = (1<<2),
		EF_DISABLE   = (1<<3),
	};

	const char* m_Name;
	const char* m_ClassName;

	Entity* m_Parent;
	std::shared_ptr<ModelBase> m_Model;

	// Object transform matrix.
	glm::mat4 m_XForm;

	// Position, rotation and scale.
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	// Entity flags.
	uint32_t m_EntityFlags;

};

#endif // !ENTITY_H
