#ifndef ENGINE_H
#define ENGINE_H

class Application;
struct GLFWwindow;

class Engine
{
public:
	void Initialize(Application* app);
	void Shutdown();
	void Frame(float dt);

	void StartGame(const char* levelname);

	void RenderFrame();

public:
	Application* m_app;
};

extern Engine g_engine;

#endif // !ENGINE_H
