#pragma once 

#include "SceneObject.hpp"
#include "visual/primitives/SphereMesh.hpp"
#include "collision/NullCollider.hpp"

class Sun : public SceneObject {
private:
	void setUniformLight(SceneObject* gameObject) const;

public:
	Sun(float radius);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void update(const std::vector<SceneObject*>& sceneObjects) override;
};

