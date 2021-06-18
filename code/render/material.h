#ifndef MATERIAL_H
#define MATERIAL_H

#include "render/shader.h"
#include "render/texture.h"

class Material
{
public:
	static void CreateMaterialFromImport(const char* name, const char* diffuseName);

public:
	Material();
	~Material();

	void Load(const std::string& filename);
	void Bind();

	std::string& GetName() { return m_materialName; }
	std::shared_ptr<Shader>& GetShader() { return m_shader; }

private:
	std::shared_ptr<Shader> m_shader;

	std::shared_ptr<Texture2D> m_albedoTexture;
	std::shared_ptr<Texture2D> m_normalTexture;
	std::shared_ptr<Texture2D> m_detailTexture;

	std::string m_materialName;

	bool m_depthWrite;
	bool m_selfillum;
};

#endif // !MATERIAL_H
