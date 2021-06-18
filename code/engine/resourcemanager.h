#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include "render/material.h"
#include "render/model.h"
#include "render/texture.h"
#include "render/shader.h"

// resource manager.
class ResourceManager
{
public:
	std::shared_ptr<Texture2D> LoadTexture2D(const std::string& name);
	std::shared_ptr<Texture2D> LoadTexture2D(const std::string& name, const TextureCreationDesc& desc);
	std::shared_ptr<TextureBase> LoadTextureCubemap(const std::string& name);
	std::shared_ptr<ModelBase> LoadModel(const std::string& name);
	std::shared_ptr<Material> LoadMaterial(const std::string& name);
	std::shared_ptr<Shader> LoadShader(const std::string& name);
	std::shared_ptr<Shader> LoadShaderFromFile(const std::string& name);

	void HotReloadShaders();
	void HotReloadMaterials();

	void Shutdown();

private:
	void FreeMaterials();
	void FreeModels();
	void FreeTextures();

private:
	typedef std::unordered_map<std::string, std::shared_ptr<Material>>::iterator MatInter;
	typedef std::unordered_map<std::string, std::shared_ptr<Shader>>::iterator ShdInter;
	typedef std::unordered_map<std::string, std::shared_ptr<ModelBase>>::iterator MdlInter;
	typedef std::unordered_map<std::string, std::shared_ptr<Texture2D>>::iterator TexInter;

	std::unordered_map<std::string, std::shared_ptr<Texture2D>> m_Textures;
	std::unordered_map<std::string, std::shared_ptr<ModelBase>> m_Models;
	std::unordered_map<std::string, std::shared_ptr<Material>> m_Materials;
	std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;

};

extern std::shared_ptr<ResourceManager> g_resourceManager;
extern std::shared_ptr<Material> g_defaultMaterial;
extern std::shared_ptr<Texture2D> g_defaulTexture;
extern std::shared_ptr<Texture2D> g_notFoundTexture;

#endif // !RESOURCEMANAGER_H
