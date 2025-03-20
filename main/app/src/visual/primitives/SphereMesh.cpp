#include "SphereMesh.hpp"



SphereMesh::SphereMesh(unsigned int N, unsigned int M, float _radius) : Surface (N, M) {
	radius = _radius;
}

void SphereMesh::eval(float x, float y, glm::vec3& pos, glm::vec3& norm) {
	float u = x * 2 * 3.14159f;
	float v = y * 3.14159f;

	pos.x = radius * glm::sin(u) * glm::cos(v);
	pos.y = radius * glm::sin(u) * glm::sin(v);
	pos.z = radius * glm::cos(u);

	norm = glm::normalize(pos);
}
