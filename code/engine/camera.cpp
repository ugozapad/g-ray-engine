#include "stdafx.h"
#include "camera.h"
#include "application.h"
#include "engine.h"
#include "input.h"

CameraManager g_cameraManager;

Camera* CameraManager::GetActiveCamera()
{
	if (m_ActiveCamera)
		return m_ActiveCamera;

	return &m_DefaultCamera;
}

void CameraManager::Update(float dt)
{
	if (m_ActiveCamera)
		m_ActiveCamera->Update(dt);
}

Camera::Camera()
{
	m_Position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_Front = glm::vec3(0.0f, 0.0f, -1.0f);
	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);

	m_Fov = 70.0f;
	m_NearZ = 0.1f;
	m_FarZ = 100.0f;
}

Camera::~Camera()
{

}


void Camera::Update(float dt)
{

}

glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

glm::mat4 Camera::GetProjectionMatrix()
{
	int width, height;
	glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);

	if (width <= 0 || height <= 0)
		return glm::perspective(glm::radians(70.0f),
			(float)1024 / (float)768,
			m_NearZ,
			m_FarZ);

	return glm::perspective(glm::radians(70.0f),
		(float)width / (float)height,
		m_NearZ,
		m_FarZ);
}

