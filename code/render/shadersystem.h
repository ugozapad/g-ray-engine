#ifndef SHADERSYSTEM_H
#define SHADERSYSTEM_H

#include "render/shader.h"

class ShaderSystem
{
public:
	void Init();
	void Shutdown();

	Shader* CreateShader(const char* name);
	Shader* CreateShaderFromFile(const char* name);

private:
	struct ShaderCreationInfo 
	{
		std::vector<VertexDeclaration> vertexDecl;

		std::string name;
		std::string vspath;
		std::string fspath;
	};

	typedef std::vector<ShaderCreationInfo>::iterator ShaderCreationInfoIt;
	std::vector<ShaderCreationInfo> m_shadersCreationInfo;

	typedef std::vector<std::pair<std::string, std::string>>::iterator ReassignTableIt;
	std::vector<std::pair<std::string, std::string>> m_reassignTable;
};

extern ShaderSystem g_shaderSystem;

#endif // !SHADERSYSTEM_H
