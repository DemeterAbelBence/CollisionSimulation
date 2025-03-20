#pragma once

#include "VertexBufferLayout.hpp"

VertexBufferLayout::VertexBufferLayout() : stride(0) {}

void VertexBufferLayout::push(unsigned int type, unsigned int count) {
	elements.push_back({ count, type, GL_FALSE });
	stride += VertexBufferElement::getSizeOfType(type) * count;
}
