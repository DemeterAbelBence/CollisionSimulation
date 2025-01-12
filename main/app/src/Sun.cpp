#include "Sun.hpp"

void Sun::setUniformLight(SceneObject* sceneObject) const {
	GpuProgram* program = sceneObject->getMesh()->getProgram();
	program->bind();
	program->setVec3(std::string("light"), bodyStateSolver->Body.X);
}

Sun::Sun(float radius) : SceneObject() {
	mesh = new SphereMesh(30, 30, radius);
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
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void Sun::resetBodyState() {}

void Sun::updateBodyState(float dt) {}

void Sun::update(const std::vector<SceneObject*>& sceneObjects) {
	for (SceneObject* s : sceneObjects) {
		setUniformLight(s);
	}
}