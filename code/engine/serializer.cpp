#include "stdafx.h"
#include "serializer.h"
#include "game/world.h"

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.SetStyle(EmitterStyle::Flow);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};
}

YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
{
	out << YAML::Flow;
	out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
	return out;
}

void Serializer::Load(const char* filename, World* world, std::string& skyboxname)
{
	std::string newfilename = filename;
	newfilename += ".level";

	std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenReadStream("levels", newfilename);
	std::string filedata = ReadStreamToString(stream);
	stream.release();

	//m_Node = YAML::LoadFile(filename);
	m_Node = YAML::Load(filedata);
	int worldVersion = m_Node["WorldVersion"].as<int>();
	skyboxname = m_Node["SkyboxModel"].as<std::string>();
	auto entities = m_Node["Entities"];

	if (!entities)
		Error("Level consist zero entities!");

	for (auto entity : entities)
	{
		std::string name = entity["Entity"].as<std::string>();
		std::string classname = entity["ClassName"].as<std::string>();

		Entity* ent = world->CreateEntity(classname.c_str());
		ent->m_Name = strdup(name.c_str());

		m_Entity = ent;

		ent->Load(this);
	}
}

void Serializer::BeginSave()
{
	m_Emitter << YAML::BeginMap;
	m_Emitter << YAML::Key << "WorldVersion" << YAML::Value << 1;
	m_Emitter << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
}

void Serializer::Save(std::unique_ptr<DataStream>& stream)
{
	m_Emitter << YAML::EndSeq;
	m_Emitter << YAML::EndMap;

	stream->Write((void*)m_Emitter.c_str(), m_Emitter.size());
}

void Serializer::SaveEntity(Entity* entity)
{
	m_Emitter << YAML::BeginMap;
	m_Emitter << YAML::Key << "Entity" << YAML::Value << entity->m_Name;
	m_Emitter << YAML::Key << "ClassName" << YAML::Value << entity->m_ClassName;

	entity->Save(this);

	m_Emitter << YAML::EndMap;
}

void Serializer::WriteString(const std::string& name, const std::string& value)
{
	m_Emitter << YAML::Key << name << YAML::Value << value;
}

void Serializer::ReadString(const std::string& name, std::string& value)
{
	auto entities = m_Node["Entities"];
	for (auto entity : entities)
	{
		std::string entname = entity["Entity"].as<std::string>();
		if (strcmp(m_Entity->m_Name, entname.c_str()) == 0)
			value = entity[name].as<std::string>();
	}	
}

void Serializer::WriteVec3(const std::string& name, const glm::vec3& vec)
{
	m_Emitter << YAML::Key << name << YAML::Value << vec;
}

void Serializer::ReadVec3(const std::string& name, glm::vec3& vec)
{
	auto entities = m_Node["Entities"];
	for (auto entity : entities)
	{
		std::string entname = entity["Entity"].as<std::string>();
		if (strcmp(m_Entity->m_Name, entname.c_str()) == 0)
			vec = entity[name].as<glm::vec3>();
	}
}
