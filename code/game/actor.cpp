#include "stdafx.h"
#include "actor.h"
#include "engine/camera.h"
#include "camerafps.h"
#include "engine/input.h"
#include "weaponrail.h"
#include "render/model.h"
#include "render/material.h"
#include "objectfactory.h"
#include "render/device.h"

Actor::Actor()
{
	m_camera = new CameraFPS();
	g_cameraManager.SetActiveCamera(m_camera);

	m_rail = (CustomWeapon*)GetObjectFactory()->CreateObject("WeaponRail");
	m_rail->SetParent(this);
	m_rail->m_Model = g_resourceManager->LoadModel("data/models/railgun.dae");
	m_rail->m_Position = glm::vec3(m_camera->GetPosition().x + 0.2f, m_camera->GetPosition().y - 0.1f, m_camera->GetPosition().z + 0.1f);
	m_rail->m_Scale *= 0.2f;
}

Actor::~Actor()
{
	delete m_rail;

	g_cameraManager.SetActiveCamera(0);
	delete m_camera;
}

void Actor::Update(float dt)
{
	inherited::Update(dt);
	m_camera->Update(dt);

	m_Position = m_camera->GetPosition();

	CameraFPS* camerafps = dynamic_cast<CameraFPS*>(m_camera);
	if (camerafps)
	{
		m_Rotation.y = -camerafps->yaw;
		m_rail->m_Rotation.z = camerafps->pitch * 1.2f;
		//m_rail->m_Position.y = -(sin(camerafps->pitch) * cos(camerafps->pitch)) * 0.0005f;
	}

	// Update Rail
	m_rail->Update(dt);

	// Shot!
	if (Input::Get().GetKey(GLFW_MOUSE_BUTTON_1))
		m_rail->MakeShot();
}

void Actor::Render()
{
	inherited::Render();
	m_rail->Render();
}
