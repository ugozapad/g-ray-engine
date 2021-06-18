#ifndef IMGUI_MGR_H
#define IMGUI_MGR_H

class ImguiMgr
{
public:
	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();

private:
	void Draw();
};

extern ImguiMgr g_imguiMgr;

#endif // !IMGUI_MGR_H
