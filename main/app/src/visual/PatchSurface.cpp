#include "PatchSurface.hpp"

PatchSurface::PatchSurface(float _size, unsigned int _rezolution) {
    rezolution = _rezolution;
	size = _size;
}

glm::vec3 PatchSurface::generateVertexData(float x, float y) {
    glm::vec3 position = glm::vec3(x, 0.0f, y);
    return position;
}

void PatchSurface::create() {
	unsigned int r = rezolution;
	float s = size;

	for (unsigned int i = 0; i < r; i++) {
		for (unsigned int j = 0; j < r; j++) {
			vertexData.push_back(generateVertexData(s * (float)i / r, s * (float)j / r));
			vertexData.push_back(generateVertexData(s * (float)(i + 1) / r, s * (float)j / r));
			vertexData.push_back(generateVertexData(s * (float)i / r, s * (float)(j + 1) / r));
			vertexData.push_back(generateVertexData(s * (float)(i + 1) / r, s * (float)(j + 1) / r));
		}
	}
	setBufferData((const void*)&vertexData[0], vertexData.size() * sizeof(glm::vec3));
}

void PatchSurface::draw(const Camera& camera) const {
    vertexArray.bind();
	
	program->bind();
	program->setMat4("MI", transformation->makeModelInverseMatrix());
	program->setMat4("M", transformation->makeModelMatrix());
	program->setMat4("V", camera.getView());
	program->setMat4("P", camera.getProjection());
	program->setVec3("eye", camera.getEye());
	setAllUniforms();

	glPatchParameteri(GL_PATCH_VERTICES, 4);
	glPolygonMode(GL_FRONT_AND_BACK, primitiveType);
    glDrawArrays(GL_PATCHES, 0, 4 * rezolution * rezolution);
}

void PatchSurface::setBufferData(const void* data, unsigned int size) {
	vertexBuffer.setData(data, size);
	vertexBufferLayout.push(GL_FLOAT, 3);
	vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);
}
