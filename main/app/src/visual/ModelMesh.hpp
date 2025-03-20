#pragma once

#include "Mesh.hpp"
#include "../util/asim/asim.hpp"

class ModelMesh : public Mesh {
private:
	asim::AssimpModel* assimpModel;

public:
	ModelMesh(const std::string& filePath, bool centerMeshes);
	void scaleModelBy(const glm::vec3& scaling);
	void create() override;
	void draw(const Camera& camera) const override;
	~ModelMesh();
};
