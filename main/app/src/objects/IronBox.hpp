#pragma once

#include "SceneObject.hpp"
#include "../visual/ModelMesh.hpp"
#include "../collision/NullCollider.hpp"

class IronBox : public SceneObject {
public:
	static ModelMesh* modelMesh;
	static void initializeModelMesh();
	static void cleanupModelMesh();

public:
	IronBox(float size, bool stationary);
	void resetBodyState() override;
	void updateBodyState(float dt) override;
	void draw(const Camera& camera) const override;
	void setMeshResources(GpuProgram* program, Material* material, const std::vector<tex::Texture*>& textures) override;
	void lightUpMesh(const Light& light) override;
};