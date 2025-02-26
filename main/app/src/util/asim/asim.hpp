#pragma once

#include <vector>
#include <algorithm>
#include <tuple>
#include <glm.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stb_image.h> 

#include "../Camera.hpp"
#include "../Transformation.hpp"

namespace asim {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec2 TexCoords;
	};

	class AssimpMesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;

	private:
		unsigned int VAO, VBO, EBO;

	private:
		void centerToOrigin(std::vector<Vertex>& vertices);

	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices);
		void downScaleMeshBy(const glm::vec3& scaling);
		void draw(const GpuProgram& program);
		void setupMesh();
	};

	class AssimpModel {
	private:
		std::vector<AssimpMesh> meshes;

	private:
		void loadModel(const std::string& filePath);
		void processNode(aiNode* node, const aiScene* scene);
		AssimpMesh processMesh(aiMesh* mesh, const aiScene* scene);

	public:
		AssimpModel(const std::string& path);
		void downScaleMeshesBy(const glm::vec3& scaling);
		void draw(const GpuProgram& program);
	};
}