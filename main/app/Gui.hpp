#pragma once

#include "src/util/gl/Debug.hpp"
#include "src/visual/primitives/Rect.hpp"

#include "imgui/imGui.h"
#include "imgui/imGui_impl_glfw.h"
#include "imgui/imGui_impl_opengl3.h"

#include "Scene.hpp"

#include <iostream>
#include <Windows.h>
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

class Gui {

private:
	static ImGuiIO* io;
	static Scene* scene;

private:
	static void renderRectangle(glm::vec3 color);
	static void renderObjectPicker();
	static void renderPrimitivePicker();
	static void renderFrameRateBox();
	static void renderProjectionSettings();
	static void renderViewSettings();
	static void renderRotationSettings();
	static void renderSimulationSettings();
	static void renderTerrainSettings();
	static void renderTesselationPicker();

public:
	static void initialize(GLFWwindow* window, Scene* _scene);
	static void render(int sceneWidth, int windowWidth, int windowHeight);
};