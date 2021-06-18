#include "stdafx.h"
#include "render/screenquad.h"

#include "render/device.h"
#include "render/shadersystem.h"

IVertexBuffer* ScreenQuad::ms_vertexBuffer;
Shader* ScreenQuad::ms_screenQuadShader;

void ScreenQuad::init()
{
	float quadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f
	};

	ms_vertexBuffer = g_renderDevice->CreateVertexBuffer(quadVertices, sizeof(quadVertices), BufferAccess::Static);
	ms_screenQuadShader = g_shaderSystem.CreateShader("quad");
}

void ScreenQuad::shutdown()
{
	delete ms_screenQuadShader;
	mem_delete(ms_vertexBuffer);
}

#include <imgui.h>

void ScreenQuad::render(ITexture2D* texture)
{
	ASSERT(texture);

	g_renderDevice->DepthTest(false);

	g_renderDevice->SetVertexBuffer(ms_vertexBuffer);
	g_renderDevice->SetTexture2D(0, texture);

	ms_screenQuadShader->Bind();
	g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
}

void ScreenQuad::render(ITexture2D* texture, Shader* shader)
{
	ASSERT(texture);
	ASSERT(shader);

	g_renderDevice->DepthTest(false);

	g_renderDevice->SetVertexBuffer(ms_vertexBuffer);
	g_renderDevice->SetTexture2D(0, texture);

	shader->Bind();
	g_renderDevice->DrawArray(PM_TRIANGLES, 0, 6);
}
