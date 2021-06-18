#include "stdafx.h"
#include "editor/editorwindow.h"
#include "engine/engine.h"

#include <imgui.h>

void EditorWindow::Render()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

