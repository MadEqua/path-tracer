#pragma once

#include "Vec3.h"
#include "Ray.h"

class Camera
{
public:
	Camera(const Vec3 &position, const Vec3 &lookAt, const Vec3 vUp, 
		float vFov, float aspectRatio,
		float aperture, float focusDistance);
	~Camera();

	//x, y are the near plane coords, between 0 and 1
	Ray getRay(float x, float y) const;
	
private:
	Vec3 position;

	//"near plane" data in world coords
	Vec3 lowerLeftCorner;
	Vec3 horizontalAxis;
	Vec3 verticalAxis;

	//Camera axis in world coords
	Vec3 cameraX, cameraY, cameraZ;

	float lensRadius;
};