#pragma once

#include "SceneObject.hpp"
#include "visual/ModelMesh.hpp"
#include "collision/NullCollider.hpp"

class IronBox : public SceneObject {

public:
	IronBox(float size, bool stationary);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void update(const std::vector<SceneObject*>& sceneObjects) override;
};