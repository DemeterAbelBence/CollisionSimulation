#pragma once

#include "VertexArray.hpp"

VertexArray::VertexArray() {
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);
}  

void VertexArray::addBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& layout) const{
	bind();
	vertexBuffer.bind();
	const auto& elements = layout.getElements();
	int offset = 0;

	for (int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];

		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type,
			element.normalized, layout.getStride(), (const void*)offset);

		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const {
	glBindVertexArray(id);
}

void VertexArray::unBind() const {
	glBindVertexArray(0);
}

VertexArray::~VertexArray() {
	glDeleteBuffers(1, &id);
}
