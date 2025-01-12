#pragma once

#include "VertexBuffer.hpp"

VertexBuffer::VertexBuffer() {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

VertexBuffer::VertexBuffer(const void* data, unsigned int size) {
	glGenBuffers(1, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::setData(const void* data, unsigned int size) {
	bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::bind() const {
	glBindBuffer(GL_ARRAY_BUFFER, id);
}

void VertexBuffer::unBind() const {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer(){
	glDeleteBuffers(1, &id);
}
