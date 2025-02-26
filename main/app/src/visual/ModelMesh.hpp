#pragma once

#include "Mesh.hpp"
#include "../util/asim/asim.hpp"

class ModelMesh : public Mesh {
private:
	asim::AssimpModel* assimpModel;
	std::vector<Texture*> textures;

public:
	ModelMesh(const std::string& filePath) : Mesh() {
		assimpModel = new asim::AssimpModel(filePath);
	}

	using TEXFILES = const std::vector<std::tuple<std::string, std::string>>&;
	void addTextures(const std::string& folder, TEXFILES texFiles) {
		for (const auto& texFile : texFiles) {
			std::string fileName = std::get<0>(texFile);
			std::string texName = std::get<1>(texFile);
			Texture* texture = new Texture(texName);
			texture->load(folder + "/" + fileName);
			textures.push_back(texture);
		}
	}

	void scaleModelBy(const glm::vec3& scaling) {
		assimpModel->downScaleMeshesBy(scaling);
	}

	void create() override {}

	void draw(const Camera& camera) const override {
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

		for (unsigned int i = 0; i < textures.size(); i++) {
			glActiveTexture(GL_TEXTURE0 + i);
			program->setInt(textures[i]->getName().c_str(), i);
			textures[i]->bind();
		}
		glActiveTexture(GL_TEXTURE0);

		assimpModel->draw(*program);
	}

	~ModelMesh() {
		delete program;
		delete material;
		delete texture;
		delete transformation;

		delete assimpModel;
	}
};
