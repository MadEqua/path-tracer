#include "Scene.h"

#include "HitRecord.h"
#include "Object.h"
#include "Material.h"
#include "Camera.h"

Scene::Scene() {
}

Scene::~Scene() {
	for (const Object *obj : objects)
		delete obj;
	for (const Material *mat : materials)
		delete mat;
	delete camera;
}

bool Scene::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const {
	HitRecord recordTemp;
	bool hit = false;
	float closestHit = tMax;

	for (const Object *obj : objects) {
		if (obj->hit(ray, tMin, closestHit, recordTemp)) {
			hit = true;
			closestHit = recordTemp.t;
			hitRecord = recordTemp;
		}
	}
	return hit;
}
