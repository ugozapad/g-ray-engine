#ifndef POSTPROCESS_H
#define POSTPROCESS_H

#include "render/shader.h"
#include "render/vertexbuffer.h"
#include "render/framebuffer.h"

struct PostProcessDesc
{
	std::shared_ptr<Shader> m_Shader;

	void Use();
};

class PostProcessRenderer
{
public:
	void Init();
	void Shutdown();

	void BeginFrame();
	void EndFrame();

	void Render();
	void Render(Framebuffer* framebuffer);

	void AddPostProcess(const std::string& name);
private:
	std::unordered_map<std::string, PostProcessDesc> m_PostProcess;
	PostProcessDesc m_GammaCorrectionPP;
	PostProcessDesc m_TestPP;

	Framebuffer* m_Framebuffer;
	IVertexBuffer* m_ScreenQuad;


};

extern std::shared_ptr<PostProcessRenderer> g_PostProcessRenderer;

#endif // !POSTPROCESS_H
