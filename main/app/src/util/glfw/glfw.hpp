#pragma once

#include <iostream>
#include <GLFW/glfw3.h>

namespace glfw {

	class Key {
	private:
		unsigned int id;
		bool pressed;

	public:
		Key(unsigned int _id);
		bool isPressed(GLFWwindow* window);
		bool isReleased(GLFWwindow* window);

	private:
		unsigned int getState(GLFWwindow* window);
	};
};