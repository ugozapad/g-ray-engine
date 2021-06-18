#include "stdafx.h"
#include "postprocess.h"
#include "device.h"
#include "engine/engine.h"
#include "engine/resourcemanager.h"
#include "render.h"

std::shared_ptr<PostProcessRenderer> g_PostProcessRenderer = std::make_shared<PostProcessRenderer>();

void PostProcessRenderer::Init()
{
	float quadVertices[] = {
	  -1.0f,  1.0f,  0.0f, 1.0f,
	  -1.0f, -1.0f,  0.0f, 0.0f,
	   1.0f, -1.0f,  1.0f, 0.0f,

	  -1.0f,  1.0f,  0.0f, 1.0f,
	   1.0f, -1.0f,  1.0f, 0.0f,
	   1.0f,  1.0f,  1.0f, 1.0f
	};

	m_ScreenQuad = g_renderDevice->CreateVertexBuffer(quadVertices, sizeof(quadVertices), BufferAccess::Static);
//	m_Framebuffer = g_RenderDevice->CreateFramebuffer(g_Engine.m_Width, g_Engine.m_Height, "postprocess", true);

	m_GammaCorrectionPP.m_Shader = g_resourceManager->LoadShader("gamma_cor");
	m_TestPP.m_Shader = g_resourceManager->LoadShader("test_pp");
}

void PostProcessRenderer::Shutdown()
{
	delete m_Framebuffer;
	delete m_ScreenQuad;
}

void PostProcessRenderer::BeginFrame()
{
//	g_renderDevice->SetFramebuffer(m_Framebuffer);
}

void PostProcessRenderer::EndFrame()
{
	g_renderDevice->SetFramebuffer(NULL);
	g_renderDevice->DepthTest(false);
	g_renderDevice->SetVertexBuffer(m_ScreenQuad);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	m_GammaCorrectionPP.m_Shader->Bind();
	glBindTexture(GL_TEXTURE_2D, m_Framebuffer->m_Texture);
	g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
}

void PostProcessRenderer::Render()
{

}

void PostProcessRenderer::Render(Framebuffer* framebuffer)
{
	g_renderDevice->DepthTest(false);

	// Render screen quad
	g_renderDevice->SetVertexBuffer(m_ScreenQuad);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	g_renderDevice->ClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	g_renderDevice->Clear(IRenderDevice::CLEAR_RT);

	m_TestPP.Use();
	glBindTexture(GL_TEXTURE_2D, framebuffer->m_Texture);
	g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
}

void PostProcessRenderer::AddPostProcess(const std::string& name)
{

}

void PostProcessDesc::Use()
{
	m_Shader->Bind();
}
