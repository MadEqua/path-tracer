#pragma once

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform
{
public:
	Transform();

	void yaw(float angle);
	void pitch(float angle);
	void roll(float angle);

	void rotate(float angle, const glm::vec3 &axis);
	void rotate(const glm::quat &quaternion);

	void scale(const glm::vec3 &scale);

	void translate(const glm::vec3 &trans);

	void setOrientation(const glm::quat &quaternion);
	void setOrientation(float w, float x, float y, float z);
	void setScale(const glm::vec3 &scale);
	void setPosition(const glm::vec3 &pos);

	inline const glm::vec3& getPosition() const { return position; }
	inline const glm::quat& getOrientation() const { return orientation; }
	inline const glm::vec3& getScale() const { return _scale; }

	void lookAt(const glm::vec3 &pos, const glm::vec3 &center, const glm::vec3 &up);

	//Call when done changing attributes
	void computeMatrices();

	const glm::mat4& getLocalToWorldMatrix();
	const glm::mat4& getWorldToLocalMatrix();
	const glm::mat3& getNormalMatrix();

private:
	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 _scale;

	glm::mat4 localToWorldMatrix;
	glm::mat4 worldToLocalMatrix;
	
	//Local to world suitable for multiplying vectors
	//If there are only uniform scales it's the same as localToWorldMatrix
	glm::mat3 normalMatrix;
};
