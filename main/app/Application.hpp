#pragma once

#include "Gui.hpp"
#include "src/util/tex/Texture.hpp"
#include "src/util/glfw/Time.hpp"

class Application {
public:
	static const unsigned int windowWidth = 1200;
	static const unsigned int windowHeight = 800;
	static const unsigned int sceneWidth = 800;
	static const unsigned int sceneHeight = 800;

	static Scene* scene;
	static GLFWwindow* window;

private:
	static GLFWwindow* initGlfwWindow(const char* windowName);
	static void initOpenGl();

public:
	static void initialize();
	static void render();
	static void close();
};