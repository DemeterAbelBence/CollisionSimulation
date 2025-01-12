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

	inline void setT(const glm::mat4& value) { T = value; }
	inline void setR(const glm::mat4& value) { R = value; }
	inline void setS(const glm::mat4& value) { S = value; }

	inline glm::mat4 getT() const { return T; }
	inline glm::mat4 getR() const { return R; }
	inline glm::mat4 getS() const { return S; }

	inline glm::vec3 getTranslation() const { return translation; }
	inline glm::vec3 getScaling() const { return scaling; }
	inline glm::vec3 getRotationAxis() const { return rotationAxis; }
	inline float getRotationAngle() const { return rotationAngle; }

	static glm::mat4 makeTranslationMatrix(glm::vec3 _translation);
	static glm::mat4 makeScaleMatrix(glm::vec3 _scaling);
	static glm::mat4 makeRotationMatrix(glm::vec3 _rotationAxis, float _rotationAngle);

	void setTranslationMarix(glm::vec3 _translation);
	void setRotationMarix(glm::vec3 _rotationAxis, float _rotationAngle);
	void setScaleMatrix(glm::vec3 _scaling);

	glm::mat4 makeModelMatrix() const;
	glm::mat4 makeModelInverseMatrix() const;
};