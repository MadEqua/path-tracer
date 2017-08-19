#include "Ray.h"

Ray::Ray(const glm::vec3 &origin, const glm::vec3 &direction) :
	origin(origin), direction(direction) {
}

Ray::Ray(const Ray &other, const glm::mat4 &transform) {
	origin = transform * glm::vec4(other.origin, 1.0f);
	direction = transform * glm::vec4(other.direction, 0.0f);
}

void Ray::set(const glm::vec3 &origin, const glm::vec3 &direction) { 
	this->origin = origin;
	this->direction = direction;
}