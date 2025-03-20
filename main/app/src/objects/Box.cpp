#include "Box.hpp"

Box::Box(float width, float height, float length, bool stationary) : SceneObject() {
	mesh = new CuboidMesh(width, height, length);
	collider = new CuboidCollider(width, height, length);

	float w = width;
	float h = height;
	float l = length;

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

	transformation = new Transformation();
	mesh->setTransformation(transformation);
	mesh->create();
	collider->setTransformation(transformation);
}

void Box::resetBodyState() {
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void Box::updateBodyState(float dt) {
	auto sides = dynamic_cast<CuboidCollider*>(collider)->getTransSides();
	bodyStateSolver->computeTotalTorque({ sides[0].center, sides[2].center, sides[4].center });
	bodyStateSolver->updateState(dt);
}