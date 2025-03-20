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
#include "../tex/tex.hpp"

namespace asim {
	struct Vertex {
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 BiTangent;
		glm::vec2 TexCoords;
	};

	class AssimpMesh {
	public:
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<tex::Texture> textures;

	private:
		unsigned int VAO, VBO, EBO;

	private:
		void centerToOrigin(std::vector<Vertex>& vertices);

	public:
		AssimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<tex::Texture> textures, bool centerMeshes);
		void downScaleMeshBy(const glm::vec3& scaling);
		void draw(const GpuProgram& program);
		void setupMesh();
	};

	class AssimpModel {
	private:
		std::vector<AssimpMesh> meshes;
		std::string directory;
		std::vector<tex::Texture> texturesLoaded;

	private:
		bool centerMeshes = false;

	private:
		void loadModel(const std::string& filePath);
		void processNode(aiNode* node, const aiScene* scene);
		AssimpMesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<tex::Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);

	public:
		AssimpModel(const std::string& path, bool _centerMeshes);
		void downScaleMeshesBy(const glm::vec3& scaling);
		void draw(const GpuProgram& program);
	};
}