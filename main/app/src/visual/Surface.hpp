#pragma once

#include <vector>

#include "Mesh.hpp"
#include "../util/Camera.hpp"

class Surface : public Mesh {
private:
	struct VertexData {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 tex_coord;
	};

private:
	unsigned int verticesPerStrip;
	unsigned int strips;
	float size;

	std::vector<VertexData> vertexData;

private:
	void setBufferData(const void* data, unsigned int size);

public:
	Surface(unsigned int N, unsigned int M);
	inline void setSize(float value) { size = value; }

	VertexData generateVertexData(float x, float y);

	void create() override;
	void draw(const Camera& camera) const override;

	virtual void eval(float x, float y, glm::vec3& pos, glm::vec3& norm) = 0;
};