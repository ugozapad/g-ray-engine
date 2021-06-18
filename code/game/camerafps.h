#ifndef CAMERAFPS_H
#define CAMERAFPS_H

#include "engine/camera.h"

class Actor;

class CameraFPS : public Camera
{
	friend class Actor;
public:
	CameraFPS();
	~CameraFPS();

	void Update(float dt);

	float yaw = -90.0f;
	float pitch = 0.0f;

private:
	glm::vec3 m_Direction;

};

#endif // !CAMERAFPS_H
