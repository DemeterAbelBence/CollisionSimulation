#include "IronBox.hpp"

IronBox::IronBox(float size, bool stationary) : SceneObject() {
	
	//mesh
	ModelMesh* modelMesh = new ModelMesh("main/app/cube/Cube_obj.obj");
	modelMesh->addTextures("main/app/cube", {
		std::make_tuple("Cube_ambient.jpg", "texture_ambient"),
		std::make_tuple("Cube_diffuse.jpg", "texture_diffuse"),
		std::make_tuple("Cube_specular.jpg", "texture_specular"),
		std::make_tuple("Cube_normal.jpg", "normal_map")
	});
	modelMesh->scaleModelBy(glm::vec3(0.045f, 0.045f, 0.045f));
	mesh = modelMesh;

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
	mesh->setTransformation(transformation);
	collider->setTransformation(transformation);
}

void IronBox::resetBodyState() {
	bodyStateSolver->rollbackToInitial();
	updateTransformations();
}

void IronBox::updateBodyState(float dt) {
	auto sides = dynamic_cast<CuboidCollider*>(collider)->getTransSides();
	bodyStateSolver->computeTotalTorque({ sides[0].center, sides[2].center, sides[4].center });
	bodyStateSolver->updateState(dt);
}

void IronBox::update(const std::vector<SceneObject*>& sceneObjects) {}
