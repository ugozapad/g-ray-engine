#include "stdafx.h"
#include "gltextures.h"
#include "render/gl_exts.h"

GLenum GetGLFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::FMT_RGB: return GL_RGB;
	case ImageFormat::FMT_RGBA: return GL_RGBA;
	case ImageFormat::FMT_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
	case ImageFormat::FMT_DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;

	case ImageFormat::FMT_RGB32F: return GL_RGB32F;
	case ImageFormat::FMT_RGBA32F: return GL_RGBA32F;
	case ImageFormat::FMT_RGB16F: return GL_RGB16F;
	case ImageFormat::FMT_RGBA16F: return GL_RGBA16F;

	case ImageFormat::FMT_SRGB: return GL_SRGB;


	}

	return 0;
}

GLenum GetGLInternalFormat(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::FMT_RGB: return GL_RGB;
	case ImageFormat::FMT_RGBA: return GL_RGBA;
	case ImageFormat::FMT_DEPTH24_STENCIL8: return GL_DEPTH24_STENCIL8;
	case ImageFormat::FMT_DEPTH32_STENCIL_8_F: return GL_DEPTH32F_STENCIL8;
	case ImageFormat::FMT_RGB32F: return GL_RGB;
	case ImageFormat::FMT_RGBA32F: return GL_RGBA;
	case ImageFormat::FMT_RGB16F: return GL_RGB;
	case ImageFormat::FMT_RGBA16F: return GL_RGBA;
	case ImageFormat::FMT_SRGB: return GL_RGB;
	}

	return 0;
}

GLenum GetGLTextureDataType(ImageFormat format)
{
	switch (format)
	{
	case ImageFormat::FMT_RGB:
	case ImageFormat::FMT_RGBA: 
		return GL_UNSIGNED_BYTE;

	case ImageFormat::FMT_RGB16F:
	case ImageFormat::FMT_RGBA16F:
	case ImageFormat::FMT_RGB32F:
	case ImageFormat::FMT_RGBA32F: 
		return GL_FLOAT;

	}

	return 0;
}


glTexture2D::glTexture2D(const TextureCreationDesc& desc) :
	m_desc(desc),
	m_handle(0)
{
	glGenTextures(1, &m_handle);
	glBindTexture(GL_TEXTURE_2D, m_handle);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 
				 0, 
				 GetGLFormat(m_desc.m_format), 
				 m_desc.m_width, m_desc.m_height, 
				 0, 
				 GetGLInternalFormat(m_desc.m_format), 
			   //GL_UNSIGNED_BYTE, 
				 GetGLTextureDataType(m_desc.m_format),
				 m_desc.m_data);

	if (m_desc.m_mipmapping)
	{
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

#define ENABLE_TEST_ANISOTROPIC_FILTERING

#ifdef ENABLE_TEST_ANISOTROPIC_FILTERING

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY, 16);
#endif // ENABLE_TEST_ANISOTROPIC_FILTERING

	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
}

glTexture2D::~glTexture2D()
{
	Release();
}

void glTexture2D::Release()
{
	if (m_handle != 0)
		glDeleteTextures(1, &m_handle);
}
