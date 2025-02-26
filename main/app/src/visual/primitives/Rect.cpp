#include "Rect.hpp"

Rect::Rect(glm::vec2 corner1, glm::vec2 corner2, glm::vec3 _color) {
	color = _color;

	shader = new GpuProgram("Rectangle Shader");
	shader->readFragmentSource("main/app/shaders/rectFragment.shader");
	shader->readVertexSource("main/app/shaders/rectVertex.shader");
	shader->createProgram();
	shader->bind();
	shader->setVec3("rectColor", color);

	glm::vec2 corner3;
	corner3.x = corner1.x;
	corner3.y = corner2.y;

	glm::vec2 corner4;
	corner4.x = corner2.x;
	corner4.y = corner1.y;

	std::vector<glm::vec2> vertexData;
	vertexData.push_back(corner1);
	vertexData.push_back(corner2);
	vertexData.push_back(corner3);

	vertexData.push_back(corner4);
	vertexData.push_back(corner2);
	vertexData.push_back(corner1);

	vertexBuffer.setData((const void*)&vertexData[0], vertexData.size() * sizeof(glm::vec2));
	bufferLayout.push(GL_FLOAT, 2);
	vertexArray.addBuffer(vertexBuffer, bufferLayout);
}

void Rect::draw() const {
	vertexArray.bind();
	shader->bind();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}
