#include "Transform.h"

#include <glm/gtc/matrix_transform.hpp>


Transform::Transform() : 
	position(0.0f),
	_scale(1.0f) {
}

void Transform::yaw(float angle) {
	rotate(angle, glm::vec3(0.0f, 1.0f, 0.0f));
}

void Transform::pitch(float angle) {
	rotate(angle, glm::vec3(1.0f, 0.0f, 0.0f));
}

void Transform::roll(float angle) {
	rotate(angle, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Transform::rotate(float angle, const glm::vec3 &axis) {
	glm::quat q = glm::angleAxis(glm::radians(angle), axis);
	rotate(q);
}

void Transform::rotate(const glm::quat &quaternion) {
	//avoid drift
	glm::quat normQuat = glm::normalize(quaternion);
	orientation = orientation * normQuat;
}

void Transform::scale(const glm::vec3 &scale) {
	_scale *= scale;
}

void Transform::translate(const glm::vec3 &trans) {
	position += orientation * trans;
}

void Transform::setOrientation(const glm::quat &quaternion) {
	orientation = quaternion;
}

void Transform::setOrientation(float w, float x, float y, float z) {
	orientation.w = w;
	orientation.x = x;
	orientation.y = y;
	orientation.z = z;
}

void Transform::setScale(const glm::vec3 &scale) {
	_scale = scale;
}

void Transform::setPosition(const glm::vec3 &pos) {
	position = pos;
}

void Transform::lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up) {
	position = pos;

	glm::vec3 dir = glm::normalize(pos - center);
	glm::vec3 side = glm::normalize(glm::cross(up, dir));
	glm::vec3 newUp = glm::normalize(glm::cross(dir, side));

	glm::mat3 rot(side, newUp, dir);
	orientation = glm::quat(rot);
}

const glm::mat4& Transform::getLocalToWorldMatrix() {
	return localToWorldMatrix;
}

const glm::mat4& Transform::getWorldToLocalMatrix() {
	return worldToLocalMatrix;
}

const glm::mat3& Transform::getNormalMatrix() {
	return normalMatrix;
}

void Transform::computeMatrices() {

	glm::mat4 rotationMatrix(orientation);

	localToWorldMatrix = glm::mat4();
	localToWorldMatrix = glm::translate(localToWorldMatrix, position);
	localToWorldMatrix = localToWorldMatrix * rotationMatrix;
	localToWorldMatrix = glm::scale(localToWorldMatrix, _scale);

	worldToLocalMatrix = glm::mat4();
	worldToLocalMatrix = glm::scale(worldToLocalMatrix, 1.0f / _scale);
	worldToLocalMatrix = worldToLocalMatrix * glm::transpose(rotationMatrix);
	worldToLocalMatrix = glm::translate(worldToLocalMatrix, -position);

	if (_scale.x == _scale.y && _scale.x == _scale.z) {
		normalMatrix = glm::mat3(localToWorldMatrix);
	}
	else {
		normalMatrix = glm::transpose(glm::mat3(worldToLocalMatrix));
	}
}