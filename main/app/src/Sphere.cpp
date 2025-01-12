#include "Sphere.hpp"

Sphere::Sphere(float _radius) : SceneObject() {
	radius = _radius;

	mesh = new SphereMesh(30, 30, radius);
	collider = new SphereCollider(radius);

	float inverseMass = 0.01f;
	float inertiaScalar = 2.0f / 5.0f * (1.0f / inverseMass) * radius * radius;
	glm::mat3 Ibody = glm::mat3(
		glm::vec3(inertiaScalar, 0.0f, 0.0f), 
		glm::vec3(0.0f, inertiaScalar, 0.0f), 
		glm::vec3(0.0f, 0.0f, inertiaScalar)
	);
	glm::mat3 Ibodyinv = glm::inverse(Ibody);

	auto body = RigidBody::BodyData{
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
		glm::vec3(0.0f)         // torque
	};
	bodyStateSolver = new RigidBody::BodyStateSolver(body);
	bodyStateSolver->hasResistance = true;

	transformation = new Transformation();
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void Sphere::resetBodyState() {
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void Sphere::updateBodyState(float dt) {
	glm::vec3 c = dynamic_cast<SphereCollider*>(collider)->getTransCenter();
	glm::vec3 v = c + radius * glm::vec3(0.0f, 0.0f, 1.0f);
	bodyStateSolver->computeTotalTorque({ v });
	bodyStateSolver->updateState(dt);
}

void Sphere::update(const std::vector<SceneObject*>& sceneObjects) {}