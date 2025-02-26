#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {

private:
	unsigned int id;

public:
	VertexArray();
	void addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) const;
	void bind() const;
	void unBind() const;
	~VertexArray();
};