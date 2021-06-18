#ifndef SHADER_H
#define SHADER_H

struct VertexDeclaration
{
	enum
	{
		VD_FLOAT,
		VD_INTEGER
	};

	std::string name;
	uint32_t type;
	uint32_t size;
	uint32_t offset;
};

class Shader
{
public:
	Shader();
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::vector<VertexDeclaration>& vertexDecls);
	~Shader();
	bool Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	void Destroy();
	void Bind();

	void SetInteger(const char* name, int value);
	void SetBoolean(const char* name, bool value);
	void SetFloat(const char* name, float value);
	void SetVec2(const char* name, const glm::vec2& vec);
	void SetVec3(const char* name, const glm::vec3& vec);
	void SetVec4(const char* name, const glm::vec4& vec);
	void SetMatrix(const char* name, const glm::mat4& matrix);

	std::string& GetName() { return m_Name; }

	void HotReload();

private:
	std::vector<VertexDeclaration> m_vertexDeclarations;

	std::string m_Name;
	std::string m_VSName;
	std::string m_FSName;
	GLuint m_Program;

	size_t m_allVxDeclsSize;
};

#endif // !SHADER_H
