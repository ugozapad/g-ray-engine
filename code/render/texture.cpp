#include "stdafx.h"
#include "texture.h"
#include "filesystem/filesystem.h"
#include "device.h"
#include "gl_exts.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Texture2D::Texture2D(const std::string& filename)
//{
//	//stbi_set_flip_vertically_on_load(true);
//
//    int width, height, channels;
//    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);
//
//	m_InfoDesc.m_Width = width;
//	m_InfoDesc.m_Height = height;
//
//	GLenum format;
//	if (channels == 1)
//		format = GL_RED;
//	else if (channels == 3)
//		format = GL_RGB;
//	else if (channels == 4)
//		format = GL_RGBA;
//
//	if (width % 2 != 0 || height % 2 != 0)
//		Msg("texture %s has non power of two size %ix%i", filename.c_str(), width, height);
//
//    glGenTextures(1, &m_TextureHandle);
//    glBindTexture(GL_TEXTURE_2D, m_TextureHandle);
//
//    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//    glGenerateMipmap(GL_TEXTURE_2D);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//#define ENABLE_TEST_ANISOTROPIC_FILTERING
//
//#ifdef ENABLE_TEST_ANISOTROPIC_FILTERING
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
//#endif // ENABLE_TEST_ANISOTROPIC_FILTERING
//
//
//    glBindTexture(GL_TEXTURE_2D, 0);
// 
//    stbi_image_free(data);
//
//	Msg("loaded %s", FileSystem::GetFileName(filename).c_str());
//}

#include "render/gl/gltextures.h"

Texture2D::Texture2D(const std::string& filename)
{
	TextureCreationDesc desc = {0};
	desc.m_mipmapping = true;

	CreateTexture(filename, desc);
}

Texture2D::Texture2D(const std::string& filename, const TextureCreationDesc& desc)
{
	CreateTexture(filename, desc);
}

Texture2D::~Texture2D()
{
	Release();
}

void Texture2D::CreateTexture(const std::string& filename, const TextureCreationDesc& desc)
{
	TextureCreationDesc texdesc;
	int width, height, channels;
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &channels, 0);

	if (width % 2 != 0 || height % 2 != 0)
	{
		Error("texture %s has non power of two size %ix%i", filename.c_str(), width, height);
		Msg("using EXT_texture_rectangle");

		texdesc.m_isNPOT = true;
	}

	texdesc.m_width = width;
	texdesc.m_height = height;
	texdesc.m_format = (channels == 4) ? ImageFormat::FMT_RGBA : ImageFormat::FMT_RGB;
	texdesc.m_data = data;
	texdesc.m_mipmapping = desc.m_mipmapping;
	m_texture2D = g_renderDevice->CreateTexture2D(texdesc);

	stbi_image_free(data);

	Msg("loaded %s", IFileSystem::Instance()->GetFileName(filename).c_str());
}

void Texture2D::Release()
{
	if (m_texture2D)
	{
		m_texture2D->Release();
		mem_delete(m_texture2D);
	}
}

void Texture2D::Bind()
{
	throw std::logic_error("The method or operation is not implemented.");
}

GLuint Texture2D::GetHandle()
{
	throw std::logic_error("The method or operation is not implemented.");
}

TextureCubemap::TextureCubemap(const std::string& filename)
{
	static std::string tex_faces_str[] = { "px", "nx", "py", "ny", "pz", "nz" };

	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

	for (int i = 0; i < 6; i++)
	{
		int32_t width, height, channels;

		char buffer[260];
		sprintf(buffer, "%s_%s.png", filename.c_str(), tex_faces_str[i].c_str());

		if (!IFileSystem::Instance()->Exist(buffer))
			Error("TextureCubemap::TextureCubemap: Couldnt find face '%s' of cubemap.", IFileSystem::Instance()->GetFileName(buffer).c_str());

		uint8_t* p_tex_data = stbi_load(buffer, &width, &height, &channels, 0);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, p_tex_data);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

TextureCubemap::~TextureCubemap()
{
	Release();
}

void TextureCubemap::Release()
{
	glDeleteTextures(1, &m_Texture);
}

void TextureCubemap::Bind()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
}

GLuint TextureCubemap::GetHandle()
{
	return m_Texture;
}


std::shared_ptr<Texture2D> TextureCreator::CreateTexture2D(const std::string& name)
{
	return std::make_shared<Texture2D>(name);
}

std::shared_ptr<Texture2D> TextureCreator::CreateTexture2D(const std::string& name, const TextureCreationDesc& desc)
{
	return std::make_shared<Texture2D>(name, desc);
}

std::shared_ptr<TextureBase> TextureCreator::CreateTextureCubemap(const std::string& name)
{
	return std::make_shared<TextureCubemap>(name);
}
