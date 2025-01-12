#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GLEW/glew.h>
#include <glm.hpp>

#include "GpuProgram.hpp"

class TesselationProgram : public GpuProgram {

private:
	std::string tesselationControlSource;
	std::string tesselationEvaluationSource;

public:
	TesselationProgram(std::string _programName) : GpuProgram(_programName) {}
	TesselationProgram(const TesselationProgram& program);
	void readTesselationEvaluationSource(const std::string& filePath);
	void readTesselationControlSource(const std::string& filePath);

	void createProgram() override;
};