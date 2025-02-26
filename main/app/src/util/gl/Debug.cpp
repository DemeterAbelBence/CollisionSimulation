#pragma once

#include "Debug.hpp"

std::string Debug::debugSourceToString(GLenum source) {
	return std::string("");
}

std::string Debug::debugTypeToString(GLenum type) {
	return std::string("");
}

std::string Debug::debugSeverityToString(GLenum severity) {
	return std::string("");
}

void Debug::outputDebugMessage(GLenum source, GLenum type, GLuint id, GLenum severity,
	GLsizei length, const GLchar* message, const void* userParam) {

	std::cout << "\nOpenGl error!!!" << std::endl;
}

void Debug::initializeDebugContext() {
	GLint context_flags;
	glGetIntegerv(GL_CONTEXT_FLAGS, &context_flags);

	if (context_flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(outputDebugMessage, nullptr);
	}
}

void Debug::displayOpenGlInfo() {
	int majorVersion, minorVersion;
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	std::cout << "GL Vendor    : " << glGetString(GL_VENDOR) << '\n'
	<< "GL Renderer  : " << glGetString(GL_RENDERER) << '\n'
	<< "GL Version (string)  : " << glGetString(GL_VERSION) << '\n'
	<< "GL Version (integer) : " << majorVersion << "." << minorVersion << '\n'
	<< "GLSL Version : " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl << std::endl;
}

