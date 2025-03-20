#include "IronBox.hpp"

ModelMesh* IronBox::modelMesh = nullptr;

void IronBox::initializeModelMesh() {
	modelMesh = new ModelMesh("main/app/assets/cube/Cube_obj.obj", true);
	modelMesh->scaleModelBy(glm::vec3(0.042f, 0.042f, 0.042f));
}

void IronBox::cleanupModelMesh() {
	delete modelMesh;
	modelMesh = nullptr;
}

IronBox::IronBox(float size, bool stationary) : SceneObject() {
	mesh = nullptr;

	//body data
	float w = 6.0f;
	float h = 6.0f;
	float l = 6.0f;
	float inverseMass;
	glm::mat3 Ibody;
	glm::mat3 Ibodyinv;
	if (!stationary) {
		inverseMass = 0.01f;
		Ibody = 1.0f / (12.0f * inverseMass) * glm::mat3(
			glm::vec3(h * h + l * l, 0.0f, 0.0f),
			glm::vec3(0.0f, w * w + l * l, 0.0f),
			glm::vec3(0.0f, 0.0f, w * w + h * h)
		);

		Ibodyinv = glm::inverse(Ibody);
	}
	else {
		inverseMass = 0.0f;
		Ibody = glm::mat3(0.0f);
		Ibodyinv = glm::mat3(0.0f);
	}
	auto bodyData = RigidBody::BodyData{
		inverseMass,			// invMass
		Ibody,                  // Ibody
		Ibodyinv,               // Ibodyinv

		glm::vec3(0.0f),        // X 
		glm::mat3(1.0f),        // R 
		glm::vec3(0.0f),        // P
		glm::vec3(0.0f),        // L 

		glm::mat3(1.0f),        // Iinv 
		glm::vec3(0.0f),        // vel 
		glm::vec3(0.0f),        // omega 

		glm::vec3(0.0f, -1.0f, 0.0f),        // force
		glm::vec3(0.0f)                      // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(bodyData);

	// collider
	collider = new CuboidCollider(w, h, l);

	//transform
	transformation = new Transformation();
	glm::vec3 scaling = glm::vec3(size, size, size);
	transformation->setScaleMatrix(scaling);
	collider->setTransformation(transformation);
}

void IronBox::resetBodyState() {
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void IronBox::updateBodyState(float dt) {
	auto sides = dynamic_cast<CuboidCollider*>(collider)->getTransSides();
	bodyStateSolver->computeTotalTorque({ sides[0].center });
	bodyStateSolver->updateState(dt);
}

void IronBox::draw(const Camera& camera) const {
	modelMesh->setTransformation(transformation);
	modelMesh->draw(camera);
}

void IronBox::setMeshResources(GpuProgram* program, Material* material, const std::vector<tex::Texture*>& textures) {
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

void IronBox::lightUpMesh(const Light& light) {
	GpuProgram* program = modelMesh->getProgram();
	if (program != nullptr) {
		program->bind();
		program->setVec3(std::string("light.position"), light.position);
		program->setVec3(std::string("light.powerDensity"), light.powerDensity);
	}
}
