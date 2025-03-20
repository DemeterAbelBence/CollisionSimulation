#pragma once

#include "../../util/gl/GpuProgram.hpp"
#include "../../util/gl/VertexArray.hpp"

class Rect {

private:
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	VertexBufferLayout bufferLayout;

	GpuProgram* shader;

	glm::vec3 color;

public:
	Rect(glm::vec2 corner1, glm::vec2 corner2, glm::vec3 _color);
	void draw() const;
	~Rect() { delete shader; }
};