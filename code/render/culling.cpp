#include "stdafx.h"
#include "render/culling.h"

OcclusionCulling* OcclusionCulling::Instance()
{
	static OcclusionCulling s_occlusionCulling;
	return &s_occlusionCulling;
}

bool OcclusionCulling::IsVisible(Camera* camera, AABB* aabb, glm::vec3& pos)
{
	if (!camera || !aabb)
		return false;

	return true;
}

