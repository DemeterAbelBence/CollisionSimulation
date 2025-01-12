#pragma once

#include "Mesh.hpp"
#include "../util/gl/TesselationProgram.hpp"

class PatchSurface : public Mesh {

private:
	unsigned int rezolution;
	float size;

	std::vector<glm::vec3> vertexData;

public:
	PatchSurface(float _size, unsigned int _rezolution);
	inline void setSize(float value) { size = value; }
	glm::vec3 generateVertexData(float x, float y);

	void create() override;
	void draw(const Camera& camera) const override;

private:
	void setBufferData(const void* data, unsigned int size);
};