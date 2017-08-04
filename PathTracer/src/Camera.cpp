#include "Camera.h"

#include "Utils.h"
#include <cmath>


Camera::Camera(const Vec3 &position, const Vec3 &lookAt, const Vec3 vUp,
	float vFov, float aspectRatio,
	float aperture, float focusDistance) : position(position) {

	lensRadius = aperture / 2.0f;

	float theta = Utils::degToRad(vFov);
	float halfHeight = tan(theta / 2.0f);
	float halfWidth = aspectRatio * halfHeight;

	cameraZ = position - lookAt;
	cameraZ.normalize();

	cameraX = vUp.cross(cameraZ);
	cameraX.normalize();

	cameraY = cameraZ.cross(cameraX);

	lowerLeftCorner = position - 
		halfWidth * focusDistance * cameraX - 
		halfHeight * focusDistance * cameraY - 
		focusDistance * cameraZ;

	horizontalAxis = 2.0f * halfWidth * focusDistance * cameraX;
	verticalAxis = 2.0f * halfHeight * focusDistance * cameraY;
}

Camera::~Camera() {
}

Ray Camera::getRay(float x, float y) const {
	Vec3 radius = lensRadius * Utils::randomInUnitDisk();
	Vec3 offset = cameraX * radius.x + cameraY * radius.y;
	
	return Ray(position + offset, lowerLeftCorner + x * horizontalAxis + y * verticalAxis - position - offset);
}