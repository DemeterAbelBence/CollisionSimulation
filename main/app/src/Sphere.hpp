#pragma once 

#include "SceneObject.hpp"
#include "visual/primitives/SphereMesh.hpp"
#include "collision/SphereCollider.hpp"

class Sphere : public SceneObject {
private:
	float radius;

public:
	Sphere(float _radius);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void update(const std::vector<SceneObject*>& sceneObjects) override;
};

