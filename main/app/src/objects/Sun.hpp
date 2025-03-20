#pragma once 

#include "SceneObject.hpp"
#include "../visual/primitives/SphereMesh.hpp"
#include "../visual/drawdata/Light.hpp"
#include "../collision/NullCollider.hpp"

class Sun : public SceneObject {
private:
	Light* light;

public:
	Sun(float radius, glm::vec3 powerDensity);
	Light* getLight() const { return light; }
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void lightUpScene(const std::vector<SceneObject*>& sceneObjects);
	~Sun();
};

