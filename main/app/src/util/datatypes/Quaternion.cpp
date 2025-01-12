#include "Quaternion.hpp"

Quaternion::Quaternion() {
	w = 0.0f;
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Quaternion::Quaternion(float _w, float _x, float _y, float _z) {
	w = _w;
	x = _x;
	y = _y;
	z = _z;
}

Quaternion::Quaternion(glm::vec3 axis, float angle) {
	w = glm::cos(angle / 2.0f);
	x = axis.x * glm::sin(angle / 2.0f);
	y = axis.y * glm::sin(angle / 2.0f);
	z = axis.z * glm::sin(angle / 2.0f);
}

Quaternion::Quaternion(glm::vec3 v) {
	w = 0.0f;
	x = v.x;
	y = v.y;
	z = v.z;
}

void Quaternion::operator=(Quaternion q) {
	w = q.w;
	x = q.x;
	y = q.y;
	z = q.z;
}

Quaternion Quaternion::operator+=(Quaternion q) {
	return Quaternion(
		w += q.w,
		x += q.x,
		y += q.y,
		z += q.z
	);
}

Quaternion Quaternion::operator+(Quaternion q) {
	return Quaternion(
		w + q.w,
		x + q.x,
		y + q.y,
		z + q.z
	);
}

Quaternion Quaternion::operator*(float scalar) {
	return Quaternion(
		scalar * w,
		scalar * x,
		scalar * y,
		scalar * z
	);
}

Quaternion Quaternion::operator*(Quaternion q) {
	Quaternion p = *this;

	float Re_p = p.scalar();
	float Re_q = q.scalar();
	glm::vec3 Ve_p = p.vector();
	glm::vec3 Ve_q = q.vector();

	float Re_result = Re_p * Re_q - glm::dot(Ve_p, Ve_q);
	glm::vec3 Ve_result = Re_p * Ve_q + Re_q * Ve_p + glm::cross(Ve_p, Ve_q);

	return Quaternion(
		Re_result,
		Ve_result.x,
		Ve_result.y,
		Ve_result.z
	);
}

Quaternion Quaternion::conjugate(void) {
	return Quaternion( w, -x, -y, -z );
}

float Quaternion::length() {
	return glm::sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalize() {
	float l = length();
	return Quaternion(
		w / l,
		x / l,
		y / l,
		z / l
	);
}
