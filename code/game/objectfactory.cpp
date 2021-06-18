#include "stdafx.h"
#include "game/objectfactory.h"
#include "engine/entity.h"

ObjectFactory::ObjectFactory()
{

}

void ObjectFactory::RegisterObject(const char* name, PfnCreateObject pfnCreateObject)
{
	Msg("registered %s", name);
	m_Factory.push_back(std::make_pair(name, pfnCreateObject));
}

Entity* ObjectFactory::CreateObject(const char* classname)
{
	for (int i = 0; i < m_Factory.size(); i++)
	{
		if (m_Factory[i].first && strstr(m_Factory[i].first, "class "))
		{
			// weird stuff
			std::string entityClassName = m_Factory[i].first;
			entityClassName = entityClassName.substr(6);

			if (strcmp(entityClassName.c_str(), classname) == 0)
			{
				Entity* entity = (Entity*)m_Factory[i].second();
				entity->m_ClassName = strdup(entityClassName.c_str());
				return entity;
			}
		}
		else
		{
			if (m_Factory[i].first && strcmp(m_Factory[i].first, classname) == 0)
			{
				Entity* entity = (Entity*)m_Factory[i].second();
				entity->m_ClassName = strdup(classname);
				return entity;
			}
		}
	}

	return NULL;
}

ObjectFactory* GetObjectFactory()
{
	static ObjectFactory s_ObjectFactory;
	return &s_ObjectFactory;
}
