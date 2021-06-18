#ifndef CAMERA_H
#define CAMERA_H

class Camera
{
public:
	Camera();
	~Camera();

	virtual void Update(float dt);

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

	glm::vec3 GetPosition() { return m_Position; }
	void SetPosition(glm::vec3& pos) { m_Position = pos; }
public:
	glm::vec3 m_Position;
	glm::vec3 m_Front;
	glm::vec3 m_Up;

	float m_Fov, m_NearZ, m_FarZ;
};

class CameraManager
{
public:
	void SetActiveCamera(Camera* camera) { m_ActiveCamera = camera; }
	Camera* GetActiveCamera();

	void Update(float dt);

private:
	Camera* m_ActiveCamera;
	Camera m_DefaultCamera;
};

extern CameraManager g_cameraManager;


#endif // !CAMERA_H
