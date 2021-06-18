#include "stdafx.h"
#include "render/debug_render.h"
#include "render/device.h"
#include "render/sphere.h"
#include "render/material.h"
#include "render/render.h"

DebugRender* DebugRender::Instance()
{
	static DebugRender s_debugRender;
	return &s_debugRender;
}

void DebugRender::Init()
{
	//m_sphere.create((void*)sphereVertexData, Sphere::verticesCount);
}

void DebugRender::Shutdown()
{

}

void DebugRender::DrawSphere(const glm::vec3& position)
{

}

void DebugRender::DebugMesh::create(void* buffer, size_t size)
{
	vb = g_renderDevice->CreateVertexBuffer(buffer, size, BufferAccess::Static);
	vxcount = size;
	material = g_resourceManager->LoadMaterial("default");
}

void DebugRender::DebugMesh::draw(const glm::vec3& position)
{
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, position);

	RenderContext& ctx = g_render->GetRenderContext();
	ctx.model = trans;

	g_render->SetRenderContext(ctx);
	g_render->RenderMesh(g_render->GetRenderContext(),
						 vb,
						 vxcount,
						 material->GetShader().get(),
						 material.get());
}
