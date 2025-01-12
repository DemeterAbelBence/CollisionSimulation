#pragma once

#include "GpuProgram.hpp"

std::string GpuProgram::shaderTypeToString(unsigned int type)
{
	switch (type) {
	case GL_VERTEX_SHADER:
		return std::string("vertex shader");
	case GL_FRAGMENT_SHADER:
		return std::string("fragment shader");
	case GL_TESS_EVALUATION_SHADER:
		return std::string("tesselation evaluatioln shader");
	case GL_TESS_CONTROL_SHADER:
		return std::string("tesselation control shader");
	}
	throw "invalid shader type code";
}

void GpuProgram::displayShaderCompilationResult(unsigned int type, unsigned int id) {
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE) {
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = new char[length];
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << shaderTypeToString(type) << " !!!" << std::endl;

		std::cout << message << std::endl;
		delete[] message;
	}
	else if (result == GL_TRUE) {
		std::cout << "Successful " << shaderTypeToString(type) << " compilation!" << std::endl;
	}
}

unsigned int GpuProgram::compileShader(unsigned int type, const std::string& source) {
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	displayShaderCompilationResult(type, id);

	return id;
}

void GpuProgram::readShaderSource(std::string& shader, const std::string& filePath) {
	std::ifstream stream(filePath);
	std::string line;

	while (getline(stream, line)) {
		shader += line;
		shader += '\n';
	}
}

GpuProgram::GpuProgram(const std::string& _programName) : programId(0) { 
	programName = _programName;
}
  
GpuProgram::GpuProgram(const std::string& _programName, const std::string& _vertexSource, const std::string& _fragmentSource) {
	vertexSource = _vertexSource;
	fragmentSource = _fragmentSource;
	programName = _programName;

	createProgram();
}

GpuProgram::GpuProgram(const GpuProgram& program) {
	this->vertexSource = program.vertexSource;
	this->fragmentSource = program.fragmentSource;
	this->programName = program.programName;
}

void GpuProgram::createProgram() {
	unsigned int id = glCreateProgram();

	std::cout << programName << " compilation results:" << std::endl;

	unsigned int vertexId = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentId = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
	
	std::cout << std::endl;

	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	glLinkProgram(id);
	glValidateProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);

	programId = id;
}

void GpuProgram::readVertexSource(const std::string& filePath) { readShaderSource(vertexSource, filePath); }

void GpuProgram::readFragmentSource(const std::string& filePath) { readShaderSource(fragmentSource, filePath); }

void GpuProgram::setVec3(const std::string& name, glm::vec3 value) const {
	glUniform3fv(glGetUniformLocation(programId, name.c_str()), 1, &value[0]);
}

void GpuProgram::setVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(programId, name.c_str()), x, y, z);
}

void GpuProgram::setMat4(const std::string& name, glm::mat4 mat) const {
	glUniformMatrix4fv(glGetUniformLocation(programId, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void GpuProgram::setFloat(const std::string& name, float value) const {
	glUniform1f(glGetUniformLocation(programId, name.c_str()), value);
}

void GpuProgram::setInt(const std::string& name, int value) const {
	glUniform1i(glGetUniformLocation(programId, name.c_str()), value);
}
