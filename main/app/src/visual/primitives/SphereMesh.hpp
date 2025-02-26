#pragma once

#include "../Surface.hpp";

class SphereMesh : public Surface {
private:
	float radius;

public:
	SphereMesh(unsigned int N, unsigned int M, float _radius);
	void eval(float u, float v, glm::vec3& pos, glm::vec3& norm) override;
};