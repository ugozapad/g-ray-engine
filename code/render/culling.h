#ifndef CULLING_H
#define CULLING_H

class Camera;
struct AABB;

class OcclusionCulling
{
public:
	static OcclusionCulling* Instance();

public:
	bool IsVisible(Camera* camera, AABB* aabb, glm::vec3& pos);
};

#endif // !CULLING_H
