#pragma once

#include <glm.hpp>

#include "gl/GpuProgram.hpp"

class Transformation {

private:
	glm::mat4 T;
	glm::mat4 R;
	glm::mat4 S;

private:
	glm::vec3 translation;
	glm::vec3 scaling;
	glm::vec3 rotationAxis;
	float rotationAngle;

public:
	Transformation();

	void setT(const glm::mat4& value) { T = value; }
	void setR(const glm::mat4& value) { R = value; }
	void setS(const glm::mat4& value) { S = value; }

	glm::mat4 getT() const { return T; }
	glm::mat4 getR() const { return R; }
	glm::mat4 getS() const { return S; }

	glm::vec3 getTranslation() const { return translation; }
	glm::vec3 getScaling() const { return scaling; }
	glm::vec3 getRotationAxis() const { return rotationAxis; }
	float getRotationAngle() const { return rotationAngle; }

	static glm::mat4 makeTranslationMatrix(glm::vec3 _translation);
	static glm::mat4 makeScaleMatrix(glm::vec3 _scaling);
	static glm::mat4 makeRotationMatrix(glm::vec3 _rotationAxis, float _rotationAngle);

	void setTranslationMarix(glm::vec3 _translation);
	void setRotationMarix(glm::vec3 _rotationAxis, float _rotationAngle);
	void setScaleMatrix(glm::vec3 _scaling);

	glm::mat4 makeModelMatrix() const;
	glm::mat4 makeModelInverseMatrix() const;
};