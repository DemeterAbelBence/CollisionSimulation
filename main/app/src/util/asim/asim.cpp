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

asim::AssimpMesh::AssimpMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<tex::Texture> textures, bool centerMeshes) {
	if (centerMeshes) {
		centerToOrigin(vertices);
	}
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;
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
	unsigned int ambientNr = 1;
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	for (unsigned int i = 0; i < textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		std::string number;
		std::string name = textures[i].name;
		if (name == "texture_amibent") {
			number = std::to_string(ambientNr++);
		}
		if (name == "texture_diffuse") {
			number = std::to_string(diffuseNr++);
		}
		if (name == "texture_specular") {
			number = std::to_string(specularNr++);
		}
		if (name == "texture_normal") {
			number = std::to_string(normalNr++);
		}

		program.setInt((name + number).c_str(), i);
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	glActiveTexture(GL_TEXTURE0);

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
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, BiTangent));

	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}

void asim::AssimpModel::loadModel(const std::string& filePath) {
	Assimp::Importer import;
	const aiScene * scene = import.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}
	directory = filePath.substr(0, filePath.find_last_of('/'));

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
	std::vector<tex::Texture> textures;

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

		if (mesh->mTangents) {
			glm::vec3 tang;
			tang.x = mesh->mTangents[i].x;
			tang.y = mesh->mTangents[i].y;
			tang.z = mesh->mTangents[i].z;
			vertex.Tangent = tang;

			glm::vec3 bitang = glm::cross(vertex.Normal, vertex.BiTangent);
			vertex.BiTangent = glm::normalize(bitang);
		}

		if (mesh->mTextureCoords[0]) {
			glm::vec2 tex;
			tex.x = mesh->mTextureCoords[0][i].x;	
			tex.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = tex;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vec;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		vertex.Tangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertex.BiTangent = glm::vec3(0.0f, 0.0f, 0.0f);
		vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
		std::vector<Vertex*> faceVertices;
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++) {
			unsigned int index = face.mIndices[j];
			indices.push_back(index);
			faceVertices.push_back(&vertices[index]);
		}

		glm::vec3 pos1 = faceVertices[0]->Position;
		glm::vec3 pos2 = faceVertices[1]->Position;
		glm::vec3 pos3 = faceVertices[2]->Position;

		glm::vec2 uv1 = faceVertices[0]->TexCoords;
		glm::vec2 uv2 = faceVertices[1]->TexCoords;
		glm::vec2 uv3 = faceVertices[2]->TexCoords;

		glm::vec3 edge1 = pos2 - pos1;
		glm::vec3 edge2 = pos3 - pos1;
		glm::vec2 deltaUV1 = uv2 - uv1;
		glm::vec2 deltaUV2 = uv3 - uv1;

		float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

		glm::vec3 tang, bitang;
		tang.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
		tang.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
		tang.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

		bitang.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
		bitang.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
		bitang.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

		for (const auto& v : faceVertices) {
			v->Tangent += tang;
			v->BiTangent += bitang;
			v->Tangent = glm::normalize(v->Tangent);
			v->BiTangent = glm::normalize(v->BiTangent);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<tex::Texture> ambientMaps = loadMaterialTextures(material,
			aiTextureType_AMBIENT, "texture_ambient");
		textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());

		std::vector<tex::Texture> diffuseMaps = loadMaterialTextures(material,
			aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<tex::Texture> specularMaps = loadMaterialTextures(material,
			aiTextureType_SPECULAR, "texture_specular");
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<tex::Texture> normalMaps = loadMaterialTextures(material,
			aiTextureType_HEIGHT, "texture_normal");
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
	}

	return AssimpMesh(vertices, indices, textures, centerMeshes);
}

std::vector<tex::Texture> asim::AssimpModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string name) {
	std::vector<tex::Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
		aiString str;
		mat->GetTexture(type, i, &str);
		std::string path = directory + '/' + str.C_Str();

		bool skip = false;
		for (unsigned int i = 0; i < texturesLoaded.size(); i++) {
			if (std::strcmp(texturesLoaded[i].path.c_str(), str.C_Str()) == 0) {
				textures.push_back(texturesLoaded[i]);
				skip = true;
				break;
			}
		}
		if (!skip) {  
			tex::Texture texture;
			texture.id = tex::TextureFromFile(path.c_str());
			texture.name = name;
			texture.path = path;
			textures.push_back(texture);
			texturesLoaded.push_back(texture);
		}
	}
	return textures;
}

asim::AssimpModel::AssimpModel(const std::string& path, bool _centerMeshes) {
	centerMeshes = _centerMeshes;
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
