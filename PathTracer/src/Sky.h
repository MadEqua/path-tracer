#pragma once

#include <string>
#include <glm/vec3.hpp>

//TODO add more options
class Sky
{
public:
	Sky(const std::string &file);
	~Sky();

	glm::vec3 getColor(const glm::vec3 &direction) const;

private:
	int w, h, n;
	float *skyData;
};

