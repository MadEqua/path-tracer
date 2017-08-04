#pragma once

#include <vector>
#include "Types.h"

class Ray;
class Object;
class Camera;
class Material;
struct HitRecord;

class Scene
{
public:
	Scene();
	~Scene();

	inline void addObject(const Object *object) { objects.push_back(object); }
	inline void addMaterial(const Material *material) { materials.push_back(material); }
	inline void setCamera(const Camera *camera) { this->camera = camera; }
	inline const Camera* getCamera() const { return camera; }

	inline uint32 getObjectCount() const { return objects.size(); }

	bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord) const;

private:
	std::vector<const Object*> objects;
	std::vector<const Material*> materials;
	const Camera *camera;
};

