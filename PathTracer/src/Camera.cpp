#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

#include "Utils.h"
#include <cmath>


Camera::Camera(const glm::vec3 &position, const glm::vec3 &lookAt, const glm::vec3 vUp,
	float vFov, float aspectRatio,
	float aperture, float focusDistance) : position(position) {

	lensRadius = aperture / 2.0f;

	float theta = Utils::degToRad(vFov);
	float halfHeight = tan(theta / 2.0f);
	float halfWidth = aspectRatio * halfHeight;

	cameraZ = glm::normalize(position - lookAt);
	cameraX = glm::normalize(glm::cross(vUp, cameraZ));
	cameraY = glm::cross(cameraZ, cameraX);

	lowerLeftCorner = position - 
		halfWidth * focusDistance * cameraX - 
		halfHeight * focusDistance * cameraY - 
		focusDistance * cameraZ;

	horizontalAxis = 2.0f * halfWidth * focusDistance * cameraX;
	verticalAxis = 2.0f * halfHeight * focusDistance * cameraY;
}

Ray Camera::getRay(float x, float y) const {
	glm::vec3 radius = lensRadius * glm::vec3(glm::diskRand(1.0f), 0.0f);
	glm::vec3 offset = cameraX * radius.x + cameraY * radius.y;
	
	return Ray(position + offset, lowerLeftCorner + x * horizontalAxis + y * verticalAxis - position - offset);
}