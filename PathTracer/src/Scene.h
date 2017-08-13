#pragma once

#include <vector>
#include "Types.h"

class Ray;
class Object;
class Camera;
class Material;
class Texture;
struct HitRecord;
class BvhNode;
struct RenderStatistics;

class Scene
{
public:
	Scene();
	~Scene();

	inline void addObject(const Object *object) { objects.push_back(object); }
	inline void addMaterial(const Material *material) { materials.push_back(material); }
	inline void addTexture(const Texture *texture) { textures.push_back(texture); }
	inline void setCamera(const Camera *camera) { this->camera = camera; }
	inline const Camera* getCamera() const { return camera; }

	void initializeBvh();

	inline uint32 getObjectCount() const { return objects.size(); }

	bool hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const;
	//bool boundingBox(float t0, float t1, AABB &aabb) const;

private:
	std::vector<const Object*> objects;
	std::vector<const Material*> materials;
	std::vector<const Texture*> textures;
	const Camera *camera;

	BvhNode *rootBvhNode;
};

