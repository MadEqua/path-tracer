#include "Scene.h"

#include <iostream>

#include "HitRecord.h"
#include "Object.h"
#include "Material.h"
#include "Camera.h"
#include "AABB.h"
#include "BvhNode.h"

Scene::Scene() : rootBvhNode(nullptr) {
}

Scene::~Scene() {
	delete rootBvhNode; //This will delete all objects and BvhNodes

	for (const Material *mat : materials)
		delete mat;

	delete camera;
}

void Scene::initializeBvh() {
	std::cout << "Computing Bounding Volume Hierarchy...";
	rootBvhNode = new BvhNode(objects, 0, objects.size() - 1, 0.0f, 1.0f);
	std::cout << " Done!" << std::endl << std::endl;
}

bool Scene::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {

	return rootBvhNode->hit(ray, tMin, tMax, hitRecord, statistics);

	/*
	HitRecord recordTemp;
	bool hit = false;
	float closestHit = tMax;

	for (const Object *obj : objects) {
		if (obj->hit(ray, tMin, closestHit, recordTemp, statistics)) {
			hit = true;
			closestHit = recordTemp.t;
			hitRecord = recordTemp;
		}
	}
	return hit;*/
}

/*bool Scene::boundingBox(float t0, float t1, AABB &aabb) const {
	if (objects.size() < 1) return false;

	AABB tempAABB;
	bool firstTrue = objects[0]->boundingBox(t0, t1, tempAABB);
	if (!firstTrue)
		return false;
	else
		aabb = tempAABB;

	for (size_t i = 1; i < objects.size(); ++i) {
		if (objects[i]->boundingBox(t0, t1, tempAABB))
			aabb.enclose(tempAABB);
		else
			return false;
	}
	return true;
}*/