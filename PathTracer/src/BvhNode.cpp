#include "BvhNode.h"

#include <iostream>
#include <algorithm> 

#include "Object.h"
#include "HitRecord.h"
#include "Utils.h"
#include "RenderStatistics.h"


Comparator::Comparator(int axis) : axis(axis) {
}
	
bool Comparator::operator() (const Object *i, const Object *j) {
	AABB aabbLeft, aabbRight;
	if (!i->boundingBox(0, 0, aabbLeft) || !j->boundingBox(0, 0, aabbRight)) {
		std::cout << "No AABB in Bounding Volume Hierarchy constructor." << std::endl;
	}
	return aabbLeft.min[axis] < aabbRight.min[axis];
}

BvhNode::BvhNode(std::vector<const Object*> &objects, uint32 l, uint32 r, float t0, float t1) {
	
	auto begin = objects.begin() + l;
	auto end = objects.begin() + r;

	int axis = static_cast<int>(Utils::random0To1() * 3.0f);
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
		left = new BvhNode(objects, l, l + center - 1, t0, t1);
		right = new BvhNode(objects, l + center, r, t0, t1);
	}

	AABB aabbLeft, aabbRight;
	if (!left->boundingBox(t0, t1, aabbLeft) || !right->boundingBox(t0, t1, aabbRight)) {
		std::cout << "No AABB in Bounding Volume Hierarchy constructor." << std::endl;
	}
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

bool BvhNode::boundingBox(float t0, float t1, AABB &aabb) const {
	aabb = this->aabb;
	return true;
}
