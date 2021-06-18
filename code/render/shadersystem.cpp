#include "stdafx.h"
#include "shadersystem.h"
#include "shader.h"

#include "engine/luascriptmanager.h"

ShaderSystem g_shaderSystem;

void ShaderSystem::Init()
{
	Msg("Initializing Shader System ...");

	g_luaScriptManager.DoFile("engine/r_shaders.lua");

	using namespace LuaPlus;
	LuaObject shaderTable = g_luaScriptManager.GetState().GetGlobals()["r_shaders"];

	for (LuaTableIterator it(shaderTable); it; it.Next())
	{
		ShaderCreationInfo info;
		info.name = it.GetKey().GetString();

		for (LuaTableIterator it2(it.GetValue()); it2; it2.Next())
		{
			if (strcmp(it2.GetKey().GetString(), "vertex_path") == 0)
				info.vspath = it2.GetValue().GetString();
			else if (strcmp(it2.GetKey().GetString(), "fragment_path") == 0)
				info.fspath = it2.GetValue().GetString();

			if (strcmp(it2.GetKey().GetString(), "vertex_declaration") == 0)
			{
				if (!it2.GetValue()["position"].IsNil())
				{
					LuaObject declTable = it2.GetValue()["position"];
					ASSERT(declTable.IsValid());

					VertexDeclaration decl;
					decl.name = "position";

					if (strcmp(declTable[1].GetString(), "float"))
						decl.type = VertexDeclaration::VD_FLOAT;

					decl.size = declTable[2].GetInteger();
					info.vertexDecl.push_back(decl);
				}

				if (!it2.GetValue()["normal"].IsNil())
				{
					LuaObject declTable = it2.GetValue()["normal"];
					ASSERT(declTable.IsValid());

					VertexDeclaration decl;
					decl.name = "normal";

					if (strcmp(declTable[1].GetString(), "float"))
						decl.type = VertexDeclaration::VD_FLOAT;

					decl.size = declTable[2].GetInteger();
					info.vertexDecl.push_back(decl);
				}

				if (!it2.GetValue()["texcoord0"].IsNil())
				{
					LuaObject declTable = it2.GetValue()["texcoord0"];
					ASSERT(declTable.IsValid());

					VertexDeclaration decl;
					decl.name = "texcoord0";

					if (strcmp(declTable[1].GetString(), "float"))
						decl.type = VertexDeclaration::VD_FLOAT;

					decl.size = declTable[2].GetInteger();
					info.vertexDecl.push_back(decl);
				}
			}
		}

		m_shadersCreationInfo.push_back(info);
	}

	// Re-assign table
	LuaObject reassignTable = g_luaScriptManager.GetState().GetGlobals()["r_reassign_table"];

	for (LuaTableIterator it(reassignTable); it; it.Next())
		m_reassignTable.push_back(std::make_pair(it.GetKey().GetString(), 
												 it.GetValue().GetString()));

}

void ShaderSystem::Shutdown()
{
	m_reassignTable.clear();
	m_shadersCreationInfo.clear();
}

Shader* ShaderSystem::CreateShader(const char* name)
{
	for (ReassignTableIt it = m_reassignTable.begin(); it != m_reassignTable.end(); ++it)
		if (it->first == name)
			return CreateShader(it->second.c_str());

	for (ShaderCreationInfoIt it = m_shadersCreationInfo.begin(); it != m_shadersCreationInfo.end(); ++it)
		if (it->name == name)
			return new Shader(it->name, it->vspath, it->fspath, it->vertexDecl);

	Error("ShaderSystem::CreateShader: Shader '%s' is not registered or declarated.", name);
	return NULL;
}

#include "filesystem/filesystem.h"

Shader* ShaderSystem::CreateShaderFromFile(const char* name)
{
	std::string vsname = IFileSystem::Instance()->GetPath("shaders");
	vsname += "/";
	vsname += name;
	vsname += ".vs";

	std::string fsname = IFileSystem::Instance()->GetPath("shaders");
	fsname += "/";
	fsname += name;
	fsname += ".fs";

	return New(Shader, name, vsname, fsname);
}
