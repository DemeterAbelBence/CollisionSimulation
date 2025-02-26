#pragma once

#include <glm.hpp>

struct Material {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
	float shininess;

	Material(glm::vec3 am, glm::vec3 di, glm::vec3 sp, float sh) {
		this->ambient = am;
		this->diffuse = di;
		this->specular = sp;
		this->shininess = sh;
	}

	Material(const Material& material) {
		this->ambient = material.ambient;
		this->diffuse = material.diffuse;
		this->specular = material.specular;
		this->shininess = material.shininess;
	}
};