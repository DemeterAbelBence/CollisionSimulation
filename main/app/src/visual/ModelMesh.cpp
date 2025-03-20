#include "ModelMesh.hpp"

ModelMesh::ModelMesh(const std::string& filePath, bool centerMeshes) : Mesh() {
	assimpModel = new asim::AssimpModel(filePath, centerMeshes);
}

void ModelMesh::scaleModelBy(const glm::vec3& scaling) {
	assimpModel->downScaleMeshesBy(scaling);
}

void ModelMesh::create() {}

void ModelMesh::draw(const Camera& camera) const {
	program->bind();
	program->setMat4("MI", transformation->makeModelInverseMatrix());
	program->setMat4("M", transformation->makeModelMatrix());
	program->setMat4("V", camera.getView());
	program->setMat4("P", camera.getProjection());
	program->setVec3("eye", camera.getEye());

	if (material) {
		program->setVec3("material.ambient", material->ambient);
		program->setVec3("material.diffuse", material->diffuse);
		program->setVec3("material.specular", material->specular);
		program->setFloat("material.shininess", material->shininess);
	}

	program->setInt("meshFrame", meshFrame);
	assimpModel->draw(*program);
}

ModelMesh::~ModelMesh() {
	delete program;
	delete material;
	delete transformation;

	delete assimpModel;
}