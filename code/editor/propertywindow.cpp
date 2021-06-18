#include "stdafx.h"
#include "editor/propertywindow.h"
#include "game/world.h"
#include "engine/entity.h"

#include <imgui.h>

void PositionProperty(Entity* entity)
{
	//static glm::vec3 pos = glm::vec3(entity->m_Position);

	//ImGui::SliderFloat3("Position", glm::value_ptr(pos), -20.0f, 20.0f);

	//entity->m_Position = pos;
}

void PropertyWindow::Render()
{
	//Entity* entity = g_pILevel->m_currentViewEntity;
	//rttr::type t = rttr::type::get_by_name(entity->m_ClassName);

	//ImGui::Begin("Property window");
	//float spacing = ImGui::GetStyle().ItemInnerSpacing.x;

	//char nameBuffer[256];
	//strcpy(nameBuffer, entity->m_Name);
	//ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer));

	//entity->m_Name = strdup(nameBuffer);

	//PositionProperty(entity);

	//ImGui::End();
}
