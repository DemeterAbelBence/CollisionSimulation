#include "Sun.hpp"

Sun::Sun(float radius, glm::vec3 powerDensity) : SceneObject() {
	mesh = new SphereMesh(30, 30, radius);
	collider = new NullCollider();
	light = new Light{ glm::vec3(0.0f), powerDensity };

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
	mesh->create();
	collider->setTransformation(transformation);
}

void Sun::resetBodyState() {}

void Sun::updateBodyState(float dt) {}

void Sun::lightUpScene(const std::vector<SceneObject*>& sceneObjects) {
	light->position = bodyStateSolver->Body.X;
	for (SceneObject* s : sceneObjects) {
		s->lightUpMesh(*light);
	}
}

Sun::~Sun() {
	delete mesh;
	delete collider;
	delete bodyStateSolver;
	delete transformation;
	delete light;
}
