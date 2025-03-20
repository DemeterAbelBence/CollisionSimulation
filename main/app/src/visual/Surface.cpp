#include "Surface.hpp"

Surface::Surface(unsigned int N, unsigned int M) : Mesh() {
	vertexData = std::vector<VertexData>();

	verticesPerStrip = (M + 1) * 2;
	strips = N;
	size = 1.0f;
}

Surface::VertexData Surface::generateVertexData(float x, float y) {
	glm::vec3 position;
	glm::vec3 normal;
	eval(x, y, position, normal);

	return {position, normal, glm::vec2(x, y)};
}

void Surface::create() {
	unsigned int N = strips;
	unsigned int M = verticesPerStrip / 2 - 1;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j <= M; j++) {
			vertexData.push_back(generateVertexData(size * (float)j / M, size * (float)i / N));
			vertexData.push_back(generateVertexData(size * (float)j / M, size * (float)(i + 1) / N));
		}
	}
	setBufferData((const void*)&vertexData[0], vertexData.size() * sizeof(VertexData));
}

void Surface::draw(const Camera& camera) const {
	vertexArray.bind();

	program->bind();
	program->setMat4("MI", transformation->makeModelInverseMatrix());
	program->setMat4("M", transformation->makeModelMatrix());
	program->setMat4("V", camera.getView());
	program->setMat4("P", camera.getProjection());
	program->setVec3("eye", camera.getEye());
	setAllUniforms();

	glPolygonMode(GL_FRONT_AND_BACK, primitiveType);

	for (unsigned int i = 0; i < 4 * strips; i++) 
		glDrawArrays(GL_TRIANGLE_STRIP, i * verticesPerStrip, verticesPerStrip);
}

void Surface::setBufferData(const void* data, unsigned int size) {
	vertexBuffer.setData(data, size);
	vertexBufferLayout.push(GL_FLOAT, 3);
	vertexBufferLayout.push(GL_FLOAT, 3);
	vertexBufferLayout.push(GL_FLOAT, 2);

	vertexArray.addBuffer(vertexBuffer, vertexBufferLayout);
}
