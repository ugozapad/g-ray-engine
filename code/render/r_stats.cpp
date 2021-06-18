#include "stdafx.h"
#include "r_stats.h"
#include "imgui.h"

RenderStats g_renderStats;

void RenderStats::Clean()
{
	m_verticesCount = 0;
	m_drawCalls = 0;
	m_framerate = 0.0f;
}

void RenderStats::Draw()
{
	const float PAD = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();

	m_framerate = io.Framerate;

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
	if (corner != -1)
	{
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
		ImVec2 work_size = viewport->WorkSize;
		ImVec2 window_pos, window_pos_pivot;
		window_pos.x = (corner & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
		window_pos.y = (corner & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
		window_pos_pivot.x = (corner & 1) ? 1.0f : 0.0f;
		window_pos_pivot.y = (corner & 2) ? 1.0f : 0.0f;
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
		window_flags |= ImGuiWindowFlags_NoMove;
	}
	//ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Render stats", 0, window_flags))
	{
		ImGui::Text("Vertices: %i", m_verticesCount);
		ImGui::Text("Draw calls: %i", m_drawCalls);
		ImGui::Text("Framerate: %3.0f", m_framerate);
	}
	ImGui::End();
}
