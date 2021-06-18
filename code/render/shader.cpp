#include "stdafx.h"
#include "shader.h"
#include "filesystem/filesystem.h"

GLuint CreateShader(GLenum shaderType, const std::string& filename)
{
	std::ifstream ifs(IFileSystem::Instance()->GetFilePath("shaders", filename));

	if (!ifs.is_open())
		Error("CreateShader: failed to open file %s", filename.c_str());

	std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
	const char* contentCStr = content.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &contentCStr, NULL);
	glCompileShader(shader);

	int  success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		Msg("Failed to compile shader %s\n%s", filename.c_str(), infoLog);
	}

	Msg("created shader from file %s", IFileSystem::Instance()->GetFileName(filename).c_str());
	return shader;
}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
	Create(name, vertexPath, fragmentPath);
}

Shader::Shader()
{

}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::vector<VertexDeclaration>& vertexDecls)
{
	ASSERT(!vertexDecls.empty());
	m_vertexDeclarations = vertexDecls;

	m_allVxDeclsSize = 0;
	for (int i = 0, size = m_vertexDeclarations.size(); i < size; i++)
		m_allVxDeclsSize += m_vertexDeclarations[i].size;

	Create(name, vertexPath, fragmentPath);
}

Shader::~Shader()
{
	glDeleteProgram(m_Program);
}

bool Shader::Create(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath)
{
	m_Name = name;
	m_VSName = vertexPath;
	m_FSName = fragmentPath;

	GLuint vertexShader = CreateShader(GL_VERTEX_SHADER, vertexPath);
	GLuint fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fragmentPath);

	if (vertexShader == 0 || fragmentShader == 0)
		return false;

	m_Program = glCreateProgram();
	glAttachShader(m_Program, vertexShader);
	glAttachShader(m_Program, fragmentShader);
	glLinkProgram(m_Program);
	 
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	int  success;
	char infoLog[512];
	glGetProgramiv(m_Program, GL_LINK_STATUS, &success);

	if (!success)
	{
		glGetProgramInfoLog(m_Program, 512, NULL, infoLog);
		Msg("Failed to link program %s", infoLog);
		return false;
	}

	return true;
}

void Shader::Destroy()
{
	glDeleteProgram(m_Program);
}

void Shader::Bind()
{
	glUseProgram(m_Program);

	size_t appliedOffset = 0;
	for (int i = 0, size = m_vertexDeclarations.size(); i < size; i++)
	{
		VertexDeclaration& vxdecl = m_vertexDeclarations[i];

		if (appliedOffset > 0)
			glVertexAttribPointer(i, vxdecl.size, GL_FLOAT, GL_FALSE, m_allVxDeclsSize * sizeof(float), (void*)(appliedOffset * sizeof(float)));
		else
			glVertexAttribPointer(i, vxdecl.size, GL_FLOAT, GL_FALSE, m_allVxDeclsSize * sizeof(float), (void*)0);

		glEnableVertexAttribArray(i);

		appliedOffset += vxdecl.size;
	}
}

void Shader::SetInteger(const char* name, int value)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform1i(uniformPosition, value);
}

void Shader::SetBoolean(const char* name, bool value)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform1i(uniformPosition, value);
}

void Shader::SetFloat(const char* name, float value)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform1f(uniformPosition, value);
}

void Shader::SetVec2(const char* name, const glm::vec2& vec)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform2fv(uniformPosition, 1, glm::value_ptr(vec));
}

void Shader::SetVec3(const char* name, const glm::vec3& vec)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform3fv(uniformPosition, 1, glm::value_ptr(vec));
}

void Shader::SetVec4(const char* name, const glm::vec4& vec)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniform4fv(uniformPosition, 1, glm::value_ptr(vec));
}

void Shader::SetMatrix(const char* name, const glm::mat4& matrix)
{
	GLint uniformPosition = glGetUniformLocation(m_Program, name);
	glUniformMatrix4fv(uniformPosition, 1, GL_FALSE, &matrix[0][0]);
}

void Shader::HotReload()
{
	Msg("--- HOT RELOAD %s ---", m_Name.c_str());
	Destroy();
	Create(m_Name, m_VSName, m_FSName);
}
