#include "stdafx.h"
#include "application.h"
#include "engine.h"
#include "input.h"

#include "render/imgui/imgui_impl_glfw.h"

#ifdef WIN32
HANDLE g_semaphore = NULL;
#endif // WIN32

void InitPlatform()
{
#ifdef WIN32
	g_semaphore = CreateSemaphoreA(0, 1, 1, "engine_execution_lock");

	if (GetLastError() == ERROR_ALREADY_EXISTS)
		Error("Can't open two instances of application.");
#endif // WIN32

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif // DEBUG

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

	if (strstr(GetCommandLineA(), "-gl_comp"))
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
}

Application::Application() :
	m_window(0)
{
	InitPlatform();

	//Create_Window("Engine", 1024, 768, false);
	g_engine.Initialize(this);
}

Application::~Application()
{
	g_engine.Shutdown();
}

void Application::Run()
{
	static float startTime = (float)glfwGetTime();
	static float endTime = (float)glfwGetTime();

	while (!glfwWindowShouldClose(m_window))
	{
		glfwPollEvents();

		if (Input::Instance()->GetKey(GLFW_KEY_ESCAPE))
			break;

		startTime = (float)glfwGetTime();
		float delta = startTime - endTime;

		g_engine.Frame(delta);

		endTime = startTime;
	}
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);

	Input::Instance()->KeyAction(key, action);
}

static void mouse_key_callback(GLFWwindow* window, int key, int action, int mods)
{
	ImGui_ImplGlfw_MouseButtonCallback(window, key, action, mods);

	Input::Instance()->KeyAction(key, action);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
	Input::Instance()->MousePosAction((float)xpos, (float)ypos);
}

static void character_callback(GLFWwindow* window, unsigned int codepoint)
{
	ImGui_ImplGlfw_CharCallback(window, codepoint);
}

void Application::Create_Window(const char* title, int width, int height, bool fullscreen)
{
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);

	GLFWmonitor* monitor = glfwGetPrimaryMonitor();

	int monitorWidth, monitorHeight;
	glfwGetMonitorWorkarea(monitor, 0, 0, &monitorWidth, &monitorHeight);

	//int windowPosX = (monitorWidth - width) / 2;
	//int windowPosY = (monitorHeight - height) / 2;


	int windowPosX = ((monitorWidth - width) / 2);
	int windowPosY = ((monitorHeight - height) / 2) + 17;

	glfwSetWindowPos(m_window, windowPosX, windowPosY);
	glfwSetKeyCallback(m_window, key_callback);
	glfwSetMouseButtonCallback(m_window, mouse_key_callback);
	glfwSetCursorPosCallback(m_window, cursor_position_callback);
	glfwSetCharCallback(m_window, character_callback);
}
