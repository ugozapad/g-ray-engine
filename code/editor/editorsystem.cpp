#include "stdafx.h"
#include "editor/editorsystem.h"

#include <imgui.h>

EditorSystem* EditorSystem::Instance()
{
	static EditorSystem s_editorSystem;
	return &s_editorSystem;
}

void EditorSystem::Render()
{
	m_mainWindow.Render();
	m_propertyWindow.Render();

	ImGui::ShowDemoWindow();
}
