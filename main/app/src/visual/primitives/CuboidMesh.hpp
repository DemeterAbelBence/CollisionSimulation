#pragma once 

#include <glm.hpp>
#include <array>

#include "../Mesh.hpp"
#include "../../util/Camera.hpp"

class CuboidMesh : public Mesh {

private:
	struct VertexData {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 textureCoordinate;
	};
	std::vector<VertexData> vertexData;

private:
	float height;
	float width;
	float length;

	std::array<glm::vec3, 8> points;

private:
	void createPoints();
	void createSide(const std::array<unsigned int, 4>& indices);
	void setBufferData(const void* data, unsigned int size);

public:
	CuboidMesh(float _width, float _height, float _length);
	void setResources(GpuProgram* _program, Material* _material, Texture* _texture);
	glm::vec3 calculateCenter() const;

	void create() override;
	void draw(const Camera& camera) const override;
};