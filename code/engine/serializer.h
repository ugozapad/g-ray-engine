#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "filesystem/filesystem.h"
#include "yaml-cpp/yaml.h"

#include "entity.h"

class World;

class Serializer
{
public:
	void Load(const char* filename, World* world, std::string& skyboxname);
	void BeginSave();
	void Save(std::unique_ptr<DataStream>& stream);

	void SaveEntity(Entity* entity);

	void WriteString(const std::string& name, const std::string& value);
	void ReadString(const std::string& name, std::string& value);

	void WriteVec3(const std::string& name, const glm::vec3& vec);
	void ReadVec3(const std::string& name, glm::vec3& vec);
private:
	YAML::Emitter m_Emitter;
	YAML::Node m_Node;
	Entity* m_Entity;
};



#endif // !SERIALIZER_H
