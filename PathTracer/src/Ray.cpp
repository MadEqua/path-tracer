#include "Ray.h"

Ray::Ray() {
}

Ray::Ray(const Vec3 & origin, const Vec3 & direction) :
	origin(origin), direction(direction) {
}

Ray::~Ray() {
}
