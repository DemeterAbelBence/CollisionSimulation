#pragma once

#include <glm.hpp>
#include <GLEW/glew.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

class GpuProgram {

protected:
	unsigned int programId;
	std::string programName;
	std::unordered_map<unsigned int, std::string> sources;

	std::string shaderTypeToString(unsigned int type);
	void displayShaderCompilationResult(unsigned int type, unsigned int id);
	unsigned int compileShader(unsigned int type, const std::string& source);
	void readShaderSource(std::string& shader, const std::string& filePath);

public:
	GpuProgram(const std::string& _programName);
	unsigned int getProgramId() { return programId; }

	void addShader(unsigned int type, const std::string& filePath);
	void bind() const { glUseProgram(programId); }
	void unBind() const { glUseProgram(0); }

	void setVec3(const std::string& name, glm::vec3 value) const;
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setMat4(const std::string& name, glm::mat4 value) const;
	void setFloat(const std::string& name, float value) const;
	void setInt(const std::string& name, int value) const;

	virtual void createProgram();
};


