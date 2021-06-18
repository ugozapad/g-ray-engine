#include "stdafx.h"
#include "resourcemanager.h"
#include "filesystem/filesystem.h"

#include "render/shadersystem.h"

#include <algorithm>

std::shared_ptr<ResourceManager> g_resourceManager = std::make_shared<ResourceManager>();
std::shared_ptr<Material> g_defaultMaterial;

std::shared_ptr<Texture2D> ResourceManager::LoadTexture2D(const std::string& name)
{
	if (!IFileSystem::GetFileSystem()->Exist(name.c_str()))
		Error("ResourceManager::LoadTexture: texture '%s' is not exist.", name.c_str());

	for (TexInter it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	m_Textures[name] = TextureCreator::CreateTexture2D(name);
	return m_Textures[name];
}

std::shared_ptr<Texture2D> ResourceManager::LoadTexture2D(const std::string& name, const TextureCreationDesc& desc)
{
	if (!IFileSystem::GetFileSystem()->Exist(name.c_str()))
		Error("ResourceManager::LoadTexture2D: texture '%s' is not exist.", name.c_str());

	for (TexInter it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	m_Textures[name] = TextureCreator::CreateTexture2D(name, desc);
	return m_Textures[name];
}

std::shared_ptr<TextureBase> ResourceManager::LoadTextureCubemap(const std::string& name)
{
	for (TexInter it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	//m_Textures[name] = TextureCreator::CreateTextureCubemap(name);
	return m_Textures[name];
}

std::shared_ptr<ModelBase> ResourceManager::LoadModel(const std::string& name)
{
	for (MdlInter it = m_Models.begin(); it != m_Models.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	std::shared_ptr<ModelBase> model = std::make_shared<ModelBase>();
	model->Load(name);
	m_Models[name] = model;
	return m_Models[name];
}

std::shared_ptr<Material> ResourceManager::LoadMaterial(const std::string& name)
{
	for (MatInter it = m_Materials.begin(); it != m_Materials.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	char buffer[256];
	sprintf(buffer, "%s/materials/%s.material", IFileSystem::GetFileSystem()->GetDefaultPath().c_str(), name.c_str());

	if (!IFileSystem::GetFileSystem()->Exist(buffer))
		return g_defaultMaterial;

	std::shared_ptr<Material> material = std::make_shared<Material>();
	material->Load(buffer);
	m_Materials[name] = material;
	return m_Materials[name];
}

std::shared_ptr<Shader> ResourceManager::LoadShader(const std::string& name)
{
	for (ShdInter it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	//std::string vsname;
	//vsname += FileSystem::GetDefaultPath();
	//vsname += "/shaders/";
	//vsname += name;
	//vsname += ".vs";

	//std::string fsname;
	//fsname += FileSystem::GetDefaultPath();
	//fsname += "/shaders/";
	//fsname += name;
	//fsname += ".fs";

	/*std::shared_ptr<Shader> shader = std::make_shared<Shader>(name, vsname, fsname);*/
	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(g_shaderSystem.CreateShader(name.c_str()));
	m_Shaders[name] = shader;
	return m_Shaders[name];
}

std::shared_ptr<Shader> ResourceManager::LoadShaderFromFile(const std::string& name)
{
	for (ShdInter it = m_Shaders.begin(); it != m_Shaders.end(); it++)
	{
		if (it->first == name)
			return it->second;
	}

	std::shared_ptr<Shader> shader = std::shared_ptr<Shader>(g_shaderSystem.CreateShaderFromFile(name.c_str()));
	m_Shaders[name] = shader;
	return m_Shaders[name];
}

void ResourceManager::HotReloadShaders()
{
	for (ShdInter it = m_Shaders.begin(); it != m_Shaders.end(); it++)
		it->second->HotReload();
}

void ResourceManager::HotReloadMaterials()
{
	for (MatInter it = m_Materials.begin(); it != m_Materials.end(); it++)
	{
		char buffer[256];
		sprintf(buffer, "%s/materials/%s.material", IFileSystem::GetFileSystem()->GetDefaultPath().c_str(), it->first.c_str());

		if (!IFileSystem::GetFileSystem()->Exist(buffer))
			Error("ResourceManager::HotReloadMaterials: unable to reload material because it has been deleted.", it->first.c_str());

		it->second->Load(buffer);
	}
}

void ResourceManager::Shutdown()
{
	FreeMaterials();
	FreeModels();
	FreeTextures();
}

void ResourceManager::FreeMaterials()
{
	for (MatInter it = m_Materials.begin(); it != m_Materials.end(); it++)
		it->second.reset();

	m_Materials.clear();
}

void ResourceManager::FreeModels()
{
	Msg("--- models unfreed data ---");
	for (MdlInter it = m_Models.begin(); it != m_Models.end(); it++)
	{
		Msg("%s", IFileSystem::GetFileSystem()->GetFileName(it->first).c_str());
		it->second->Destroy();
		it->second.reset();
	}

	m_Models.clear();
}

void ResourceManager::FreeTextures()
{
	Msg("--- textures unfreed data ---");
	for (TexInter it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		Msg("%s", it->first.c_str());
		it->second->Release();
		it->second.reset();
	}

	m_Models.clear();
}
