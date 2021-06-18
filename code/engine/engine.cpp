#include "stdafx.h"
#include "engine.h"
#include "application.h"
#include "render/render.h"
#include "camera.h"
#include "game/world.h"
#include "input.h"
#include "game/game.h"
#include "resourcemanager.h"
#include "filesystem/filesystem.h"
#include "render/imgui/imgui_mgr.h"
#include "render/r_stats.h"
#include "luascriptmanager.h"
#include "editor/editorsystem.h"
#include "render/deferred_renderer.h"

Engine g_engine;
bool g_editorMode = false;

extern void	CalculateBuildNumber();
extern int build_id;
extern void RenderFrame();

void Engine::Initialize(Application* app)
{
	m_app = app;

	CalculateBuildNumber();

	if (!m_app->GetWindow())
	{
		char buf[12];
		sprintf(buf, "%i", build_id);

		std::string windowTitle;
		windowTitle += "Engine | ";
		windowTitle += "build ";
		windowTitle += buf;
		windowTitle += " date ";
		windowTitle += __DATE__;

		m_app->Create_Window(windowTitle.c_str(), 1366, 768, false);
	}

	// Initialize filesystem before log.
	IFileSystem::Instance()->Initialize();

	LogInit();

	g_luaScriptManager.Init();

	g_render->Init(app);

	g_imguiMgr.Init();

	GetGame()->Init();

	g_pILevel = New(World);

	if (strstr(GetCommandLineA(), "-editor"))
		g_editorMode = true;

	if (const char* lvlname = strstr(GetCommandLineA(), "-start"))
		g_engine.StartGame(lvlname + 7);
}

void Engine::Shutdown()
{
	g_pILevel->Destroy();

	GetGame()->Shutdown();

	g_imguiMgr.Shutdown();

	g_render->Shutdown();

	g_luaScriptManager.Shutdown();

	g_resourceManager->Shutdown();

	glfwTerminate();

	LogClose();
}

void Engine::StartGame(const char* levelname)
{
	if (g_pILevel->IsLoaded())
	{
		Msg("Engine::StartGame: Game is already started");
		return;
	}

	g_pILevel->Load(levelname);
}

void Engine::ShutdownGame()
{
	if (!g_pILevel->IsLoaded())
	{
		Msg("Engine::ShutdownGame: Game is not started");
		return;
	}

	g_pILevel->Destroy();
}

#include "application.h"
#include "resourcemanager.h"
#include "render/device.h"
#include "render/postprocess.h"

#include <imgui.h>

void Engine::RenderFrame()
{
	int width, height;
	glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);

	// Set viewport.
	Viewport vp = { 0, 0, width, height };
	g_renderDevice->SetViewport(&vp);

	g_render->BeginFrame();

	g_renderDevice->SetFramebuffer(g_deferredRenderer.getFramebuffer());
	g_renderDevice->Clear(IRenderDevice::CLEAR_RT | IRenderDevice::CLEAR_DEPTH);

	// pass 0 - skybox
	g_pILevel->Render_Skybox();

	// pass 1 - geometry
	g_pILevel->Render_Geom();

	g_renderDevice->SetFramebuffer(0);
	g_renderDevice->Clear(IRenderDevice::CLEAR_RT | IRenderDevice::CLEAR_DEPTH);

	// draw result
	ScreenQuad::render(g_deferredRenderer.getTexture(2));

	// stats
	g_renderStats.Draw();

	// End imgui render and flush to render
	g_imguiMgr.EndFrame();

	// End frame
	g_render->EndFrame();
}

void Engine::Frame(float dt)
{
	// ImGui frame begin
	g_imguiMgr.BeginFrame();

	if (g_editorMode)
		EditorSystem::Instance()->Render();

	g_pILevel->Update(dt);

	RenderFrame();

	if (Input::Instance()->GetKey(GLFW_KEY_F11))
		g_render->MakeScreenShot();

	if (Input::Instance()->GetKey(GLFW_KEY_F10))
	{
		const char* currentLevelName = strdup(g_pILevel->m_levelName);
		ShutdownGame();
		Sleep(100);
		StartGame(currentLevelName);
		Sleep(100);
	}
}
