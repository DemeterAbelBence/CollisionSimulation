#pragma once

#include <glm.hpp>
#include <GLEW/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

class GpuProgram {

protected:
	unsigned int programId;
	std::string vertexSource;
	std::string fragmentSource;
	std::string programName;

	std::string shaderTypeToString(unsigned int type);
	void displayShaderCompilationResult(unsigned int type, unsigned int id);
	unsigned int compileShader(unsigned int type, const std::string& source);
	void readShaderSource(std::string& shader, const std::string& filePath);

public:
	GpuProgram(const std::string& _programName);
	GpuProgram(const std::string& _programName, const std::string& _vertexSource, const std::string& _fragmentSource);
	GpuProgram(const GpuProgram& program);
	unsigned int getProgramId() { return programId; }

	void readVertexSource(const std::string& filePath);
	void readFragmentSource(const std::string& filePath);
	inline void bind() const { glUseProgram(programId); }
	inline void unBind() const { glUseProgram(0); }

	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;

	virtual void createProgram();
};


