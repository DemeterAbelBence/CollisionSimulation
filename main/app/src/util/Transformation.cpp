#include "Transformation.hpp"

Transformation::Transformation() {
	S = glm::mat4(1.0f);
	R = glm::mat4(1.0f);
	T = glm::mat4(1.0f);
}

glm::mat4 Transformation::makeTranslationMatrix(glm::vec3 translation) {
	return glm::mat4(
		glm::vec4(1.0f, 0.0f, 0.0f, translation.x),
		glm::vec4(0.0f, 1.0f, 0.0f, translation.y),
		glm::vec4(0.0f, 0.0f, 1.0f, translation.z),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

glm::mat4 Transformation::makeScaleMatrix(glm::vec3 scaling) {
	return glm::mat4(
		glm::vec4(scaling.x, 0.0f, 0.0f, 0.0f),
		glm::vec4(0.0f, scaling.y, 0.0f, 0.0f),
		glm::vec4(0.0f, 0.0f, scaling.z, 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

glm::mat4 Transformation::makeRotationMatrix(glm::vec3 rotationAxis, float rotationAngle) {
	glm::mat4 r(1.0f);

	if (rotationAxis == glm::vec3(0.0f))
		return r;

	float a = rotationAngle;
	glm::vec3 k = rotationAxis;

	float cosA = cos(a);
	float sinA = sin(a);

	r[0][0] = cosA + k.x * k.x * (1.0f - cosA);
	r[0][1] = k.x * k.y * (1.0f - cosA) - k.z * sinA;
	r[0][2] = k.x * k.z * (1.0f - cosA) + k.y * sinA;
	r[0][3] = 0.0f;

	r[1][0] = k.y * k.x * (1.0f - cosA) + k.z * sinA;
	r[1][1] = cosA + k.y * k.y * (1.0f - cosA);
	r[1][2] = k.y * k.z * (1.0f - cosA) - k.x * sinA;
	r[1][3] = 0.0f;

	r[2][0] = k.z * k.x * (1.0f - cosA) - k.y * sinA;
	r[2][1] = k.z * k.y * (1.0f - cosA) + k.x * sinA;
	r[2][2] = cosA + k.z * k.z * (1.0f - cosA);
	r[2][3] = 0.0f;

	r[3][0] = 0.0f;
	r[3][1] = 0.0f;
	r[3][2] = 0.0f;
	r[3][3] = 1.0f;

	return r;
}

void Transformation::setTranslationMarix(glm::vec3 _translation) {
	translation = _translation;
	T = makeTranslationMatrix(_translation);
}

void Transformation::setRotationMarix(glm::vec3 _rotationAxis, float _rotationAngle) {
	rotationAxis = _rotationAxis;
	rotationAngle = _rotationAngle;
	R = makeRotationMatrix(_rotationAxis, _rotationAngle);
}

void Transformation::setScaleMatrix(glm::vec3 _scaling) {
	scaling = _scaling;
	S = makeScaleMatrix(_scaling);
}

glm::mat4 Transformation::makeModelMatrix() const {
	return S * R * T;
}

glm::mat4 Transformation::makeModelInverseMatrix() const
{
	glm::mat4 M = makeModelMatrix();
	float D = glm::determinant(M);

	if (D != 0.0f) {
		return glm::inverse(M);
	}
	else {
		std::cout << "Model matrix has no inverse!" << std::endl;
		return glm::mat4(1.0f);
	}
}
