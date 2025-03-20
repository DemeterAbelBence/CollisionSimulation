#include "Sponza.hpp"

ModelMesh* Sponza::modelMesh = nullptr;

void Sponza::initializeModelMesh() {
	modelMesh = new ModelMesh("main/app/assets/sponza/sponza.obj", false);
}

void Sponza::cleanupModelMesh() {
	delete modelMesh;
	modelMesh = nullptr;
}

Sponza::Sponza(float size) {
	mesh = nullptr;
	collider = new NullCollider();

	auto body = RigidBody::BodyData{
		0.0f,			        // invMass
		glm::mat3(1.0f),        // Ibody
		glm::mat3(1.0f),        // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P 
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f),        // force
		glm::vec3(0.0f)         // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(body);

	transformation = new Transformation();
	glm::vec3 scaling = glm::vec3(size, size, size);
	transformation->setScaleMatrix(scaling);
	collider->setTransformation(transformation);
}

void Sponza::resetBodyState() {

}

void Sponza::updateBodyState(float dt) {
	bodyStateSolver->updateState(dt);
}

void Sponza::draw(const Camera& camera) const {
	modelMesh->setTransformation(transformation);
	modelMesh->draw(camera);	
}

void Sponza::setMeshResources(GpuProgram* program, Material* material, const std::vector<tex::Texture*>& textures) {
	if (modelMesh->getProgram() == nullptr) {
		modelMesh->setProgram(program);
	}
	if (modelMesh->getMaterial() == nullptr) {
		modelMesh->setMaterial(material);
	}
	if (modelMesh->getTextures().empty()) {
		modelMesh->setTextures(textures);
	}
}

void Sponza::lightUpMesh(const Light& light) {
	GpuProgram* program = modelMesh->getProgram();
	if (program != nullptr) {
		program->bind();
		program->setVec3(std::string("light.position"), light.position);
		program->setVec3(std::string("light.powerDensity"), light.powerDensity);
	}
}


