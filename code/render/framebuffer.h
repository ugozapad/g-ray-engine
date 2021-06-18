#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

class ITexture2D;

class IFramebuffer
{
public:
	virtual ~IFramebuffer() {}

	virtual unsigned int getHandle() = 0;

	virtual void setColorTexture(int index, ITexture2D* colorTexture) = 0;
	virtual void setDepthTexture(ITexture2D* depthTexture) = 0;
};

class Framebuffer
{
public:
	Framebuffer(uint32_t width, uint32_t height, const std::string& name, bool srgb = false);
	~Framebuffer();

	void Bind();
	void UnBind();

public:
	std::string m_Name;
	uint32_t m_Width, m_Height;
	uint32_t m_Framebuffer, m_RenderBuffer, m_Texture;
};

#endif // !FRAMEBUFFER_H
