#include "Mesh.hpp"

void Mesh::drawMeshFrame(const Camera& camera) {
	transformation->setScaleMatrix(glm::vec3(1.2f, 1.2f, 1.2f));
	meshFrame = 1;
	draw(camera);

	transformation->setScaleMatrix(glm::vec3(1.0f, 1.0f, 1.0f));
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
