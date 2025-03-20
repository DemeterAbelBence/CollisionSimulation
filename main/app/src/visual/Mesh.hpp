#pragma once

#include "../util/gl/VertexArray.hpp"
#include "../util/gl/GpuProgram.hpp"
#include "../util/tex/tex.hpp"
#include "../visual/drawdata/Material.hpp"
#include "../util/Transformation.hpp"
#include "../util/Camera.hpp"

class Mesh {

protected:
	VertexArray vertexArray;
	VertexBuffer vertexBuffer;
	VertexBufferLayout vertexBufferLayout;

	GpuProgram* program;
	Material* material;
	Transformation* transformation;

	std::vector<tex::Texture*> textures;

	unsigned int primitiveType;
	unsigned int meshFrame;

public:
	Mesh();

	void setProgram(GpuProgram* _program) { program = _program; }
	void setMaterial(Material* _material) { material = _material; }
	void setTransformation(Transformation* _transformation) { transformation = _transformation; }
	void setTextures(const std::vector<tex::Texture*> _textures) { textures = _textures; }

	GpuProgram* getProgram() const { return program; }
	Material* getMaterial() const { return material; }
	Transformation* getTransformation() const { return transformation; }
	const std::vector<tex::Texture*>& getTextures() const { return textures; }

	void setPrimitive(unsigned int type) { primitiveType = type; }
	unsigned int getPrimitiveType() const { return primitiveType; }

	void setAllUniforms() const;
	void drawMeshFrame(const Camera& camera);
	
	virtual void create() = 0;
	virtual void draw(const Camera& camera) const = 0;

	virtual ~Mesh();
};