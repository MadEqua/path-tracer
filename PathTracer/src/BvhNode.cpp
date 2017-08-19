#include "BvhNode.h"

#include <algorithm> 
#include <glm/gtc/random.hpp>

#include "Object.h"
#include "HitRecord.h"
#include "Utils.h"
#include "RenderStatistics.h"


Comparator::Comparator(int axis) : axis(axis) {
}
	
bool Comparator::operator() (const Object *i, const Object *j) {
	const AABB &aabbLeft = i->getAABB();
	const AABB &aabbRight = i->getAABB();
	return aabbLeft.min[axis] < aabbRight.min[axis];
}

BvhNode::BvhNode(std::vector<const Object*> &objects, uint32 l, uint32 r) {
	
	auto begin = objects.begin() + l;
	auto end = objects.begin() + r;

	int axis = static_cast<int>(glm::linearRand(0.0f, 1.0f) * 3.0f);
	Comparator comparator(axis);
	std::sort(begin, end + 1, comparator);

	size_t n = (r - l) + 1;
	if (n == 1) {
		left = right = *begin; //no null pointers to avoid comparisions at traversal
	}
	else if (n == 2) {
		left = *begin;
		right = *end;
	}
	else {
		uint32 center = n / 2;
		left = new BvhNode(objects, l, l + center - 1);
		right = new BvhNode(objects, l + center, r);
	}

	AABB &aabbLeft = left->getAABB();
	const AABB &aabbRight = right->getAABB();
	aabbLeft.enclose(aabbRight);
	aabb = aabbLeft;
}

BvhNode::~BvhNode() {
	delete left;
	if(right != left)
		delete right;
}

bool BvhNode::hit(const Ray &ray, float tMin, float tMax, HitRecord &hitRecord, RenderStatistics &statistics) const {

	statistics.bvhIntersectionsComputed++;
	
	if (aabb.hit(ray, tMin, tMax)) {
		HitRecord leftRecord, rightRecord;
		bool hitLeft = left->hit(ray, tMin, tMax, leftRecord, statistics);
		bool hitRight = right->hit(ray, tMin, tMax, rightRecord, statistics);
		if (hitLeft && hitRight) {
			hitRecord = leftRecord.t < rightRecord.t ? leftRecord : rightRecord;
			return true;
		}
		else if(hitLeft) {
			hitRecord = leftRecord;
			return true;
		}
		else if (hitRight) {
			hitRecord = rightRecord;
			return true;
		}
	}
	statistics.bvhIntersectionsFailed++;
	return false;
}