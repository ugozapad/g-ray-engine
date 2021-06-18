#include "stdafx.h"
#include "material.h"
#include "filesystem/filesystem.h"
#include "shader.h"
#include "texture.h"
#include "device.h"

#include <sstream>

class MaterialParser
{
	const char* k_tokenizerStr = "{},\"\n\r\t ";
public:
	MaterialParser(const std::string& filename)
	{
		std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenReadStream(filename);

		stream->Seek(SeekDir::End, 0);
		size_t length = stream->Tell();
		stream->Seek(SeekDir::Begin, 0);

		m_data.reserve(length);

		stream->Read((void*)m_data.data(), length);

		m_data.push_back('\0');

		m_stream = std::istringstream(m_data);

		//m_token = strtok((char*)m_data.data(), k_tokenizerStr);
	}

	~MaterialParser()
	{

	}

	std::string GetParameter(const std::string& param_name)
	{

	}

	bool GetString(std::string& buffer)
	{
		return !!std::getline(m_stream, buffer);
	}

public:
	std::string m_data;
	std::istringstream m_stream;
	char* m_token;
};

Material::Material()
{
	m_depthWrite = true;
	m_selfillum = false;
}

Material::~Material()
{
}

void Material::Load(const std::string& filename)
{
	std::unique_ptr<DataStream> stream = IFileSystem::Instance()->OpenReadStream(filename);

	stream->Seek(SeekDir::End, 0);
	size_t length = stream->Tell();
	stream->Seek(SeekDir::Begin, 0);

	char* data = new char[length + 1];
	stream->Read((void*)data, length);
	data[length] = '\0';

	const char* albedoTextureName = NULL;
	const char* normalTextureName = NULL;
	const char* detailTextureName = NULL;
	bool disableMipMapping = false;

	const char* tokenizerStr = "{},\"\n\r\t ";
	char* token = strtok(data, tokenizerStr);
	while (token)
	{
		if (strcmp(token, "material") == 0)
		{
			char* materialName = strtok(NULL, tokenizerStr);
			m_materialName = strdup(materialName);
			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "technique") == 0)
		{
			char* technique = strtok(NULL, tokenizerStr);
			m_shader = g_resourceManager->LoadShader(technique);
			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "albedo") == 0)
		{
			char* albedoName = 0;
			char* nextTextureTok = strtok(NULL, tokenizerStr);
			if (strcmp(nextTextureTok, "skipmips") == 0)
			{
				disableMipMapping = true;
				albedoName = strtok(NULL, tokenizerStr);
			}
			else
				albedoName = nextTextureTok;

			albedoTextureName = strdup(albedoName);
			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "normal_map") == 0)
		{
			char* normalname = strtok(NULL, tokenizerStr);
			normalTextureName = strdup(normalname);
			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "detail") == 0)
		{
			char* detailname = strtok(NULL, tokenizerStr);
			detailTextureName = strdup(detailname);
			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "depth_write") == 0)
		{
			char* depthWriteValue = strtok(NULL, tokenizerStr);

			if (strcmp(depthWriteValue, "true") == 0)
				m_depthWrite = true;
			else if (strcmp(depthWriteValue, "false") == 0)
				m_depthWrite = false;

			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else if (strcmp(token, "selfillum") == 0)
		{
			char* selfillumValue = strtok(NULL, tokenizerStr);

			if (strcmp(selfillumValue, "true") == 0)
				m_selfillum = true;
			else if (strcmp(selfillumValue, "false") == 0)
				m_selfillum = false;

			token = strtok(NULL, tokenizerStr);
			continue;
		}
		else
		{
			Error("Material::Load: get unknowed token '%s' while reading '%s' file.",
				token, IFileSystem::Instance()->GetFileName(filename).c_str());
		}

		token = strtok(NULL, tokenizerStr);
	}

	delete[] data;

	TextureCreationDesc desc;
	desc.m_mipmapping = !disableMipMapping;

	std::string texturePath = IFileSystem::Instance()->GetFilePath("textures", albedoTextureName);
	m_albedoTexture = g_resourceManager->LoadTexture2D(texturePath, desc);

	if (normalTextureName)
	{
		texturePath = IFileSystem::Instance()->GetFilePath("textures", normalTextureName);
		m_normalTexture = g_resourceManager->LoadTexture2D(texturePath);
	}

	if (detailTextureName)
	{
		texturePath = IFileSystem::Instance()->GetFilePath("textures", detailTextureName);
		m_detailTexture = g_resourceManager->LoadTexture2D(texturePath);
	}
}

#include "engine/camera.h"
#include "engine/engine.h"

#include "imgui.h"
#include "render/imgui/imgui_mgr.h"

void Material::Bind()
{
	g_renderDevice->DepthTest(true);
	g_renderDevice->DepthMask(m_depthWrite);

	ASSERT(m_shader);
	m_shader->Bind();

	// activate albedo texture as 0
	g_renderDevice->SetTexture2D(0, m_albedoTexture->GetHWTexture());
	m_shader->SetInteger("u_albedoTexture", 0);

	// Set up normal texture as 1
	if (m_normalTexture)
	{
		g_renderDevice->SetTexture2D(1, m_normalTexture->GetHWTexture());
		m_shader->SetInteger("u_normalTexture", 1);
		m_shader->SetInteger("u_usingNormalMapping", 1);
	}

	// Set up detail texture as 5
	if (m_detailTexture)
	{
		g_renderDevice->SetTexture2D(5, m_detailTexture->GetHWTexture());
		m_shader->SetInteger("u_detailTexture", 5);
	}

	Camera* camera = g_cameraManager.GetActiveCamera();
	m_shader->SetFloat("u_ZNear", camera->m_NearZ);
	m_shader->SetFloat("u_ZFar", camera->m_FarZ);

	m_shader->SetInteger("u_selfillum", m_selfillum);

	static float lightPosArr[3] = {2.0f,2.0f,2.0f};
	
	glm::vec3 lightPos(lightPosArr[0], lightPosArr[1], lightPosArr[2]);
	m_shader->SetVec3("u_LightPos", lightPos);
	m_shader->SetVec3("u_LightColor", glm::vec3(1.0f, 1.0f, 1.0f));
	m_shader->SetVec3("u_ObjectColor", glm::vec3(1.0f, 0.5f, 0.31f));
}
