#ifndef APPLICATION_H
#define APPLICATION_H

class Application
{
public:
	Application();
	~Application();

	void Run();

	void Create_Window(const char* title, int width, int height, bool fullscreen);

	GLFWwindow* GetWindow() { return m_window; }

private:
	GLFWwindow* m_window;
};

#endif // !APPLICATION_H
