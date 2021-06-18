#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H

#include "render/framebuffer.h"

class glFramebuffer : public IFramebuffer
{
public:
	glFramebuffer();
	~glFramebuffer();

	unsigned int getHandle();

	void setColorTexture(int index, ITexture2D* colorTexture);
	void setDepthTexture(ITexture2D* depthTexture);

private:
	GLuint m_handle;
	GLuint m_render_buffer;
};

#endif // !GLFRAMEBUFFER_H
