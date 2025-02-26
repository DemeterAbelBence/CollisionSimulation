#include "Mesh.hpp"

void Mesh::drawMeshFrame(const Camera& camera) {
	glm::vec3 s = transformation->getScaling();
	transformation->setScaleMatrix(s + 0.15f * s);
	meshFrame = 1;
	draw(camera);

	transformation->setScaleMatrix(s);
	meshFrame = 0;
}

Mesh::Mesh() {
	program = nullptr; 
	texture = nullptr;
	material = nullptr;
	transformation = nullptr;

	meshFrame = 0;
	primitiveType = GL_TRIANGLES;
}

void Mesh::setAllUniforms() const {
	if (program) {
		program->bind();
		if (material) {
			program->setVec3("material.ambient", material->ambient);
			program->setVec3("material.diffuse", material->diffuse);
			program->setVec3("material.specular", material->specular);
			program->setFloat("material.shininess", material->shininess);
		}
		if(texture) {
			glActiveTexture(GL_TEXTURE0);
			texture->bind();
			glUniform1i(glGetUniformLocation(program->getProgramId(),
						texture->getName().c_str()), 0);
		}
		program->setInt("meshFrame", meshFrame);
	}
}

Mesh::~Mesh() {
	delete program;
	delete material;
	delete texture;
	delete transformation;
}
