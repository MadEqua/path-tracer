#pragma once

#include <glm/vec3.hpp>
#include "Ray.h"

class Camera
{
public:
	Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 vUp,
		float vFov, float aspectRatio,
		float aperture, float focusDistance);

	//x, y are the near plane coords, between 0 and 1
	Ray getRay(float x, float y) const;
	
private:
	glm::vec3 position;

	//"near plane" data in world coords
	glm::vec3 lowerLeftCorner;
	glm::vec3 horizontalAxis;
	glm::vec3 verticalAxis;

	//Camera axis in world coords
	glm::vec3 cameraX, cameraY, cameraZ;

	float lensRadius;
};