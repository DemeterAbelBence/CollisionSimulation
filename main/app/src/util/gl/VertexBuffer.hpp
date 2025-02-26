#pragma once

#include <iostream>
#include <GLEW/glew.h>

class VertexBuffer {

private:
	unsigned int id;

public:
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size);
	void setData(const void* data, unsigned int size);
	void bind() const;
	void unBind() const;
	~VertexBuffer();
};