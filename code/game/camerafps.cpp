#include "stdafx.h"
#include "camerafps.h"
#include "engine/application.h"
#include "engine/engine.h"
#include "engine/input.h"

CameraFPS::CameraFPS()
{

}

CameraFPS::~CameraFPS()
{

}

void CameraFPS::Update(float dt)
{
	int width, height;
	glfwGetWindowSize(g_engine.m_app->GetWindow(), &width, &height);



	static float lastX = (float)width / 2.0;
	static float lastY = (float)height / 2.0;

	//glfwSetInputMode(g_engine.m_app->GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	double x, y;
	glfwGetCursorPos(g_engine.m_app->GetWindow(), &x, &y);

	//glfwSetCursorPos(g_Engine.m_Window, g_Engine.m_Width / 2, g_Engine.m_Height / 2);

	const float sensitivity = 0.3f;

	float xoffset = x - lastX;
	float yoffset = lastY - y; // reversed since y-coordinates go from bottom to top
	lastX = x;
	lastY = y;

	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	m_Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_Direction.y = sin(glm::radians(pitch));
	m_Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	m_Front = glm::normalize(m_Direction);

	float speed = 5.0f * dt;
	Input& input = Input::Get();

	if (input.GetKey(GLFW_KEY_LEFT_SHIFT) ||
		input.GetKey(GLFW_KEY_RIGHT_SHIFT))
		speed += 10.0f * dt;

	if (input.GetKey(GLFW_KEY_W))
		m_Position += speed * m_Front;

	if (input.GetKey(GLFW_KEY_S))
		m_Position -= speed * m_Front;
	if (input.GetKey(GLFW_KEY_A))
		m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
	if (input.GetKey(GLFW_KEY_D))
		m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;

	if (input.GetKey(GLFW_KEY_Q))
		m_Position -= speed * glm::vec3(0.0f, -1.0f, 0);
	if (input.GetKey(GLFW_KEY_Z))
		m_Position += speed * glm::vec3(0.0f, -1.0f, 0);
}
