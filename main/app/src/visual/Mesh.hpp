#pragma once

#include "../util/gl/VertexArray.hpp"
#include "../util/gl/GpuProgram.hpp"
#include "../util/tex/Texture.hpp"
#include "../util/datatypes/Material.hpp"
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

	inline void setProgram(GpuProgram* _program) { program = _program; }
	inline void setMaterial(Material* _material) { material = _material; }
	inline void setTexture(Texture* _texture) { texture = _texture; }
	inline void setTransformation(Transformation* _transformation) { transformation = _transformation; }

	inline GpuProgram* getProgram() const { return program; }
	inline Material* getMaterial() const { return material; }
	inline Texture* getTexture() const { return texture; }
	inline Transformation* getTransformation() const { return transformation; }

	inline void setPrimitive(unsigned int type) { primitiveType = type; }
	inline unsigned int getPrimitiveType() const { return primitiveType; }

	void setAllUniforms() const;
	void drawMeshFrame(const Camera& camera);
	
	virtual void create() = 0;
	virtual void draw(const Camera& camera) const = 0;

	virtual ~Mesh();
};