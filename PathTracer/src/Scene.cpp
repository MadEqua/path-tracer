#include "Scene.h"

#include <iostream>

#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "BvhNode.h"
#include "Object.h"

Scene::Scene() : rootBvhNode(nullptr) {
}

Scene::~Scene() {
	if(rootBvhNode != nullptr)
		delete rootBvhNode; //This will delete all objects and BvhNodes

	for (const Material *mat : materials)
		delete mat;

	for (const Texture *tex : textures)
		delete tex;

	delete camera;
}

void Scene::initializeStaticData() {
	std::cout << "Computing Transform Matrices...";
	for (const Object *o : objects) {
		o->computeTransformMatrices();
	}
	std::cout << " Done!" << std::endl;

	std::cout << "Computing AABBs...";
	for (const Object *o : objects) {
		o->computeAABB();
	}
	std::cout << " Done!" << std::endl;
	
	std::cout << "Computing Bounding Volume Hierarchy...";
	rootBvhNode = new BvhNode(objects, 0, objects.size() - 1);
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