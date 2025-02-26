#include "asim.hpp"

void asim::AssimpMesh::centerToOrigin(std::vector<Vertex>& vertices) {
	glm::vec3 center = glm::vec3(0.0f, 0.0f, 0.0f);
	for (const auto& vertex : vertices) {
		center += vertex.Position;
	}
	unsigned int size = vertices.size();
	center = center / (float)size;

	for (auto& vertex : vertices) {
		vertex.Position -= center;
	}
}

asim::AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
	centerToOrigin(vertices);
	this->vertices = vertices;
	this->indices = indices;
	setupMesh();
}

void asim::AssimpMesh::downScaleMeshBy(const glm::vec3& scaling) {
	glm::mat4 S = Transformation::makeScaleMatrix(scaling);
	for (auto& vertex : vertices) {
		glm::vec4 newPosition = glm::vec4(vertex.Position, 1.0) * S;
		vertex.Position = glm::vec3(newPosition);
	}
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
}

void asim::AssimpMesh::draw(const GpuProgram& program) {
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void asim::AssimpMesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
		&indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void asim::AssimpModel::loadModel(const std::string& filePath) {
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	processNode(scene->mRootNode, scene);
}

void asim::AssimpModel::processNode(aiNode* node, const aiScene* scene) {
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++) {
		processNode(node->mChildren[i], scene);
	}
}

asim::AssimpMesh asim::AssimpModel::processMesh(aiMesh* mesh, const aiScene* scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;

		glm::vec3 pos;
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.Position = pos;

		glm::vec3 norm;
		norm.x = mesh->mNormals[i].x;
		norm.y = mesh->mNormals[i].y;
		norm.z = mesh->mNormals[i].z;
		vertex.Normal = norm;

		if (mesh->mTextureCoords[0])
		{
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = tex;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	return AssimpMesh(vertices, indices);
}

asim::AssimpModel::AssimpModel(const std::string& path) {
	loadModel(path);
}

void asim::AssimpModel::downScaleMeshesBy(const glm::vec3& scaling) {
	for (auto& mesh : meshes) {
		mesh.downScaleMeshBy(scaling);
	}
}

void asim::AssimpModel::draw(const GpuProgram& program) {
	for (unsigned int i = 0; i < meshes.size(); i++)
		meshes[i].draw(program);
}

