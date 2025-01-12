#include "DebugDrawer.hpp"

GpuProgram* DebugDrawer::program = nullptr;
unsigned int DebugDrawer::mode = GL_POINTS;
unsigned int DebugDrawer::overrideZ = 0;

std::vector<glm::vec3> DebugDrawer::vertexData = std::vector<glm::vec3>();

void DebugDrawer::initProgram() {
	program = new GpuProgram("debug_shader");
	program->readFragmentSource("main/app/shaders/debugFragment.shader");
	program->readVertexSource("main/app/shaders/debugVertex.shader");
	program->createProgram();
}

void DebugDrawer::setVertexData(const std::vector<glm::vec3>& _vertexData) {
	vertexData.clear();
	vertexData = _vertexData;
}

void DebugDrawer::draw(const Camera& camera, glm::vec3 color) {
	if (vertexData.size() == 0) {
		return;
	}

	program->bind();
	program->setMat4("V", camera.getView());
	program->setMat4("P", camera.getProjection());
	program->setVec3("color", color);
	program->setInt("overrideZ", overrideZ);

	unsigned int vertexArrayId;
	glGenVertexArrays(1, &vertexArrayId);
	glBindVertexArray(vertexArrayId);

	unsigned int vertexBufferId;
	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexData.size() * sizeof(glm::vec3), (const void*)&vertexData[0], GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);

	if (mode == GL_POINTS) {
		glPointSize(10.0f);
	}
	if(mode == GL_LINES) {
		glLineWidth(2.0f);
	}

	glDrawArrays(mode, 0, vertexData.size());

	glDeleteBuffers(1, &vertexBufferId);
	glDeleteVertexArrays(1, &vertexArrayId);
}
