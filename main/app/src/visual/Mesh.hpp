#pragma once

#include "../util/gl/VertexArray.hpp"
#include "../util/gl/GpuProgram.hpp"
#include "../util/tex/Texture.hpp"
#include "../visual/drawdata/Material.hpp"
#include "../util/Transformation.hpp"
#include "../util/Camera.hpp"

#include <glm.hpp>

class Mesh {

protected:
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	VertexBufferLayout vertexBufferLayout;

	GpuProgram* program;
	Material* material;
	Texture* texture;
	Transformation* transformation;

	unsigned int primitiveType;
	unsigned int meshFrame;

public:
	Mesh();

	void setProgram(GpuProgram* _program) { program = _program; }
	void setMaterial(Material* _material) { material = _material; }
	void setTexture(Texture* _texture) { texture = _texture; }
	void setTransformation(Transformation* _transformation) { transformation = _transformation; }

	GpuProgram* getProgram() const { return program; }
	Material* getMaterial() const { return material; }
	Texture* getTexture() const { return texture; }
	Transformation* getTransformation() const { return transformation; }

	void setPrimitive(unsigned int type) { primitiveType = type; }
	unsigned int getPrimitiveType() const { return primitiveType; }

	void setAllUniforms() const;
	void drawMeshFrame(const Camera& camera);
	
	virtual void create() = 0;
	virtual void draw(const Camera& camera) const = 0;

	virtual ~Mesh();
};