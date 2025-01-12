#pragma once

#include <iostream>
#include <vector>
#include <GLEW/glew.h>

struct VertexBufferElement {
	unsigned int count;
	unsigned int type;
	unsigned int normalized;

	static unsigned int getSizeOfType(unsigned int type) {
		switch (type) {
		case GL_FLOAT: return 4;
		}
		return 0;
	}
};

class VertexBufferLayout {

private:
	std::vector<VertexBufferElement> elements;
	unsigned int stride;

public:
	VertexBufferLayout();
	inline unsigned int getStride() const { return stride; }
	inline const std::vector<VertexBufferElement>& getElements() const { return elements; }
	void push(unsigned int type, unsigned int count);
};