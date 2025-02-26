#pragma once

#include "SceneObject.hpp"
#include "visual/primitives/CuboidMesh.hpp"
#include "collision/CuboidCollider.hpp"

class Box : public SceneObject {

public:
	Box(float width, float height, float length, bool stationary);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void update(const std::vector<SceneObject*>& sceneObjects) override;
};