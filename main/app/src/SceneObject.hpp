#pragma once

#include <deque>
#include <tuple>

#include "collision/Collider.hpp"
#include "collision/CuboidCollider.hpp"
#include "body/RigidBody.hpp"
#include "visual/Mesh.hpp"
#include "visual/DebugDrawer.hpp"
#include "util/Camera.hpp"

class SceneObject {
protected:
	Mesh* mesh;
	Collider* collider;
	RigidBody::BodyStateSolver* bodyStateSolver;
	Transformation* transformation;

public:
	std::string name;

public:
	SceneObject();

	Mesh* getMesh() const { return mesh; }
	Collider* getCollider() const { return collider; }
	RigidBody::BodyStateSolver* getBodyStateSolver() const { return bodyStateSolver; }
	Transformation* getTransformation() const { return transformation; }

	void translate(glm::vec3 transalte);
	void setRotation(glm::mat4 R);

	void setMeshResources(GpuProgram* program, Texture* texture, Material* material);
	void create();
	void draw(const Camera& camera) const;
	void updateTransformations();

	virtual void resetBodyState() = 0;
	virtual void updateBodyState(float dt) = 0;
	virtual void update(const std::vector<SceneObject*>& sceneObjects) = 0;

	virtual ~SceneObject();
};