#include "stdafx.h"
#include "game/light.h"

Light::Light()
{

}

Light::~Light()
{

}

#include "imgui.h"

void Light::Update(float dt)
{
	inherited::Update(dt);

	//ImGui::SliderFloat3("POS", glm::value_ptr(m_Position), -100.0f, 100.0f);
}

// !!! STUB FUNCTION !!!
void Light::Render()
{
	inherited::Render();
}
// !!! STUB FUNCTION !!!

void Light::Load(Serializer* serializer)
{
	inherited::Load(serializer);
}

void Light::Save(Serializer* serializer)
{
	inherited::Save(serializer);
}