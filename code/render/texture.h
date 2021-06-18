#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include "render/gfxcommon.h"

struct TextureCreationDesc
{
	int m_width;
	int m_height;
	ImageFormat m_format;
	void* m_data;
	bool m_mipmapping;
	bool m_isNPOT = false;
};

struct TextureInfoDesc
{
	int m_Width;
	int m_Height;
	ImageFormat m_Format;
};

class ITextureBase
{
public:
	virtual ~ITextureBase() {}

	virtual void Release() = 0;
	virtual uint32_t GetHandle() = 0;
};

class ITexture2D : public ITextureBase
{
};


class TextureBase
{
public:
	TextureBase() {}
	virtual ~TextureBase() {}

	virtual void Release() = 0;
	virtual void Bind() = 0;

	virtual GLuint GetHandle() = 0;

	const TextureInfoDesc& GetInfoDesc() { return m_InfoDesc; }

protected:
	TextureInfoDesc m_InfoDesc;
};

class Texture2D : public TextureBase
{
public:
	Texture2D(const std::string& filename);
	Texture2D(const std::string& filename, const TextureCreationDesc& desc);
	
	~Texture2D();

	void CreateTexture(const std::string& filename, const TextureCreationDesc& desc);

	void Release();
	ITexture2D* GetHWTexture() { return m_texture2D; }

	void Bind() override;
	GLuint GetHandle() override;

private:
	ITexture2D* m_texture2D;
	const char* m_fileName;
};

class TextureCubemap : public TextureBase
{
public:
	TextureCubemap(const std::string& filename);
	~TextureCubemap();

	void Release();
	void Bind();

	GLuint GetHandle();
private:
	GLuint m_Texture;
};

class TextureCreator
{
public:
	static std::shared_ptr<Texture2D> CreateTexture2D(const std::string& name);
	static std::shared_ptr<Texture2D> CreateTexture2D(const std::string& name, const TextureCreationDesc& desc);
	static std::shared_ptr<TextureBase> CreateTextureCubemap(const std::string& name);
};

#endif // TEXTURE_H
