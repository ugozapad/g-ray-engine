#include "stdafx.h"
#include "glrender.h"
#include "engine/camera.h"
#include "../postprocess.h"
#include "../device.h"
#include "engine/engine.h"
#include "engine/application.h"
#include "../gl_exts.h"
#include "../shadowrenderer.h"
#include "../r_stats.h"
#include "../shadersystem.h"
#include "../debug_render.h"
#include "../deferred_renderer.h"
#include "../framebuffer.h"

static glRender s_glrender;
IRender* g_render = static_cast<IRender*>(&s_glrender);

bool FoundExt(const std::string& extname)
{
	int exts;
	glGetIntegerv(GL_NUM_EXTENSIONS, &exts);

	for (int i = 0; i < exts; i++)
	{
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if (extname == extension)
		{
			Msg(" found %s", extname.c_str());
			return true;
		}
	}

	Msg(" not found %s", extname.c_str());
	return false;
}

void PrintExtensions()
{
	int exts;
	glGetIntegerv(GL_NUM_EXTENSIONS, &exts);

	for (int i = 0; i < exts; i++)
	{
		const char* extension = (const char*)glGetStringi(GL_EXTENSIONS, i);
		Msg("%s", extension);
	}
}

void APIENTRY r_glDebugOutput(GLenum source, 
                            GLenum type, 
                            unsigned int id, 
                            GLenum severity, 
                            GLsizei length, 
                            const char *message, 
                            const void *userParam)
{

	if (type != GL_DEBUG_TYPE_ERROR)
		return;

	Msg("%stype = 0x%x, severity = 0x%x, message = %s",
		(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR ** " : ""),
		type, severity, message);
}

void R_InitOpenGL()
{/*
	Msg("Initializing OpenGL ...");
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		Error("Your video card doesn't meet minimum system requirements for engine. Please, update graphics drivers or upgrade your video card");

	PrintExtensions();

	if (FoundExt("GL_ARB_debug_output"))
	{
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glfwGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glfwGetProcAddress("glGetDebugMessageLogARB");
	}

	if (!FoundExt("GL_ARB_texture_filter_anisotropic") && !FoundExt("GL_EXT_texture_filter_anisotropic"))
		Error("R_InitOpenGL: Failed to find extension 'GL_ARB_texture_filter_anisotropic'. Please, update your video card drivers.");

	if (!FoundExt("GL_ARB_program_interface_query") || !FoundExt("GL_ARB_vertex_attrib_binding") || !FoundExt("GL_ARB_shading_language_packing"))
		Error("Your video card doesn't meet minimum system requirements for engine. Please, update graphics drivers or upgrade your video card");*/

	Msg("Initializing OpenGL ...");
	gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
	//PrintExtensions();

	if (FoundExt("GL_ARB_debug_output"))
	{
		glDebugMessageControlARB = (PFNGLDEBUGMESSAGECONTROLARBPROC)glfwGetProcAddress("glDebugMessageControlARB");
		glDebugMessageInsertARB = (PFNGLDEBUGMESSAGEINSERTARBPROC)glfwGetProcAddress("glDebugMessageInsertARB");
		glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)glfwGetProcAddress("glDebugMessageCallbackARB");
		glGetDebugMessageLogARB = (PFNGLGETDEBUGMESSAGELOGARBPROC)glfwGetProcAddress("glGetDebugMessageLogARB");
	}

	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);
	glDebugMessageCallbackARB(r_glDebugOutput, 0);
}

void SetSwapInterval(int value)
{
	glfwSwapInterval(value);
	Msg("swap interval changed to %i", value);
}

Framebuffer* GetFramebuffer()
{
	glRender* render = (glRender*)g_render;
	return render->m_frameBuffer;
}

unsigned int g_VAO;

glRender::glRender()
{
	m_app = NULL;
	m_frameBuffer = NULL;

	memset(&m_Ctx, 0, sizeof(m_Ctx));
}

glRender::~glRender()
{

}

void glRender::Init(Application* app)
{
	ASSERT(app);
	m_app = app;

	Msg("Initializing renderer ...");

	glfwMakeContextCurrent((GLFWwindow*)app->GetWindow());
	SetSwapInterval(0);
	R_InitOpenGL();

	Msg("%s", glGetString(GL_VENDOR));
	Msg("%s", glGetString(GL_RENDERER));
	Msg("OpenGL ver. %s", glGetString(GL_VERSION));

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	//glFrontFace(GL_CW);

	glGenVertexArrays(1, &g_VAO);
	glBindVertexArray(g_VAO);

	g_shaderSystem.Init();
	g_defaultMaterial = g_resourceManager->LoadMaterial("default");
	ScreenQuad::init();
	g_deferredRenderer.init();
	g_shadowRenderer.Init();
	DebugRender::Instance()->Init();
}

void glRender::Shutdown()
{
	g_shadowRenderer.Shutdown();
	g_deferredRenderer.shutdown();
	ScreenQuad::shutdown();
	g_shaderSystem.Shutdown();
	glDeleteVertexArrays(1, &g_VAO);
	glfwMakeContextCurrent(0);
}

void glRender::BeginFrame()
{
	g_renderDevice->DepthTest(true);
	g_renderStats.Clean();
}

void glRender::EndFrame()
{
	glfwSwapBuffers(m_app->GetWindow());
}

#include "filesystem/filesystem.h"
#include "engine/engine.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void stbi_write_callback(void* context, void* data, int size)
{
	DataStream* stream = reinterpret_cast<DataStream*>(context);
	return stream->Write(data, size);
}

#define USE_IDTECH_LIKE_NAMING

void glRender::MakeScreenShot()
{
	int width, height;
	glfwGetWindowSize(m_app->GetWindow(), &width, &height);

	size_t bufferSize = 3 * width * height;
	uint8_t* screenBuffer = new uint8_t[bufferSize];

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

#ifndef USE_IDTECH_LIKE_NAMING
	char buffer[256];
	for (int i = 0; ; i++)
	{
		sprintf(buffer, "sshot_%i.jpg", i);
		if (!FileSystem::Exist("screenshots", buffer))
			break;
	}
#else
	time_t currentTime = time(0);
	struct tm* timeinfo = localtime(&currentTime);

	char timebuf[256];
	strftime(timebuf, sizeof(timebuf), "%d-%m-%y_%H-%M-%S", timeinfo);

	char buffer[256];
	sprintf(buffer, "ss_%s.jpg", timebuf);
#endif // USE_IDTECH_LIKE_NAMING

	std::unique_ptr<DataStream> stream = IFileSystem::GetFileSystem()->OpenWriteStream("screenshots", buffer);

	stbi_flip_vertically_on_write(true);
	stbi_write_jpg_to_func(stbi_write_callback, (void*)stream.get(), width, height, 3, screenBuffer, 100);
	stbi_flip_vertically_on_write(false);

	delete[] screenBuffer;
	stream.release();
}

#include "render/model.h"

void glRender::SetCtx()
{

}

void glRender::RenderSubMesh(const RenderContext& ctx, SubMesh* submesh, Shader* shader, Material* material /*= NULL*/)
{
	g_renderDevice->SetVertexBuffer(submesh->GetVertexBuffer());

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	g_renderDevice->SetIndexBuffer(submesh->GetIndexBuffer());

	material->Bind();

	glm::mat4 mvp = glm::mat4(1.0f);
	mvp = ctx.proj * ctx.view * ctx.model;
	shader->SetMatrix("u_modelViewProjection", mvp);

	shader->SetMatrix("u_model", ctx.model);
	shader->SetMatrix("u_view", ctx.view);
	shader->SetMatrix("u_proj", ctx.proj);
	shader->SetMatrix("u_lightSpaceMatrix", ctx.view);

	g_renderDevice->DrawElement(PM_TRIANGLES, submesh->GetIndeciesCount());

	g_renderStats.m_verticesCount += submesh->GetVerticesCount();
}

void glRender::RenderMesh(const RenderContext& ctx, 
						  IVertexBuffer* vertexbuffer, 
						  IIndexBuffer* indexbuffer, 
						  size_t indeciesCount, 
						  Shader* shader, 
						  Material* material /*= NULL*/)
{
	ASSERT(vertexbuffer);
	ASSERT(indexbuffer);
	ASSERT(shader);
	ASSERT(material);

	g_renderDevice->SetVertexBuffer(vertexbuffer);
	g_renderDevice->SetIndexBuffer(indexbuffer);

	material->Bind();

	// init params
	shader->SetMatrix("u_model", ctx.model);
	shader->SetMatrix("u_view", ctx.view);
	shader->SetMatrix("u_proj", ctx.proj);
	shader->SetMatrix("u_lightSpaceMatrix", ctx.view);

	g_renderDevice->DrawElement(PM_TRIANGLES, indeciesCount);
}

void glRender::RenderMesh(const RenderContext& ctx, 
						  IVertexBuffer* vertexbuffer, 
						  size_t verticesCount,
						  Shader* shader, 
						  Material* material /*= NULL*/)
{
	ASSERT(vertexbuffer);
	ASSERT(shader);
	ASSERT(material);

	g_renderDevice->SetVertexBuffer(vertexbuffer);
	material->Bind();

	// init params
	shader->SetMatrix("u_model", ctx.model);
	shader->SetMatrix("u_view", ctx.view);
	shader->SetMatrix("u_proj", ctx.proj);
	shader->SetMatrix("u_lightSpaceMatrix", ctx.view);

	g_renderDevice->DrawArray(PM_TRIANGLES, 0, verticesCount);
}