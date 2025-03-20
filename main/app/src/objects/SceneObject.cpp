#include "SceneObject.hpp"

SceneObject::SceneObject() {
	collider = nullptr;
	mesh = nullptr;
	bodyStateSolver = nullptr;
	transformation = nullptr;
}

void SceneObject::translate(glm::vec3 translation) {
	bodyStateSolver->Body.X += translation;
	updateTransformations();
}

void SceneObject::setRotation(glm::mat4 R) {
	bodyStateSolver->Body.R = glm::mat3(R);
	updateTransformations();
}

void SceneObject::updateTransformations() {
	transformation->setTranslationMarix(bodyStateSolver->Body.X);

	transformation->setR(glm::mat4(
		glm::vec4(bodyStateSolver->Body.R[0], 0.0f),
		glm::vec4(bodyStateSolver->Body.R[1], 0.0f),
		glm::vec4(bodyStateSolver->Body.R[2], 0.0f),
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	));

	collider->updateTransformations();
}

void SceneObject::draw(const Camera& camera) const {
	mesh->draw(camera);
}

void SceneObject::setMeshResources(GpuProgram* program, Material* material, const std::vector<tex::Texture*>& textures) {
	mesh->setProgram(program);
	mesh->setMaterial(material);
	mesh->setTextures(textures);
}

void SceneObject::lightUpMesh(const Light& light) {
	GpuProgram* program = mesh->getProgram();
	if (program != nullptr) {
		program->bind();
		program->setVec3(std::string("light.position"), light.position);
		program->setVec3(std::string("light.powerDensity"), light.powerDensity);
	}
}

SceneObject::~SceneObject() {	
	delete mesh;
	delete collider;
	delete bodyStateSolver;
	delete transformation;
}
