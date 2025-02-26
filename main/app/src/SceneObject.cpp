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

void SceneObject::setMeshResources(GpuProgram* program, Texture* texture, Material* material) {
	mesh->setProgram(program);
	mesh->setMaterial(material);
	mesh->setTexture(texture);
}

void SceneObject::create() {
	mesh->create();
}

void SceneObject::draw(const Camera& camera) const {
	mesh->draw(camera);		
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

SceneObject::~SceneObject() {	
	delete mesh;
	delete collider;
	delete bodyStateSolver;
	delete transformation;
}
