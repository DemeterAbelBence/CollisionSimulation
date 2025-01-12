#pragma once

#include "../util/gl/VertexArray.hpp"
#include "../util/gl/GpuProgram.hpp"
#include "../util/Transformation.hpp"
#include "../util/Camera.hpp"

class DebugDrawer {
private:
	static GpuProgram* program;
	static std::vector<glm::vec3> vertexData;
	static unsigned int mode;
	static unsigned int overrideZ;

public:
	static void initProgram();
	inline static void deleteProgram() { delete program; }
	inline static void setMode(unsigned int _mode) { mode = _mode; }
	inline static void setOverrideZ(unsigned int _overrideZ) { overrideZ = _overrideZ; }
	static void setVertexData(const std::vector<glm::vec3>& _vertexData);
	static void draw(const Camera& camera, glm::vec3 color);
};