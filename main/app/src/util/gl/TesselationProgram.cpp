#include "TesselationProgram.hpp"

TesselationProgram::TesselationProgram(const TesselationProgram& program) : GpuProgram(program) {
	this->tesselationControlSource = program.tesselationControlSource;
	this->tesselationEvaluationSource = program.tesselationEvaluationSource;
}

void TesselationProgram::readTesselationEvaluationSource(const std::string& filePath) {
	readShaderSource(tesselationEvaluationSource, filePath); 
}

void TesselationProgram::readTesselationControlSource(const std::string& filePath) { 
	readShaderSource(tesselationControlSource, filePath);
}

void TesselationProgram::createProgram() {
	unsigned int id = glCreateProgram();

	std::cout << programName << " compilation results:" << std::endl;

	unsigned int vertexId = compileShader(GL_VERTEX_SHADER, vertexSource);
	unsigned int fragmentId = compileShader(GL_FRAGMENT_SHADER, fragmentSource);
	unsigned int tesselationControlId = compileShader(GL_TESS_CONTROL_SHADER, tesselationControlSource);
	unsigned int tesselationEvaluationId = compileShader(GL_TESS_EVALUATION_SHADER, tesselationEvaluationSource);

	std::cout << std::endl;

	glAttachShader(id, vertexId);
	glAttachShader(id, fragmentId);
	glAttachShader(id, tesselationControlId);
	glAttachShader(id, tesselationEvaluationId);
	glLinkProgram(id);
	glValidateProgram(id);

	glDeleteShader(vertexId);
	glDeleteShader(fragmentId);
	glDeleteShader(tesselationEvaluationId);
	glDeleteShader(tesselationControlId);

	programId = id;
}