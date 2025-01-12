#include "Time.hpp"

float Time::currentTime = 0.0f;
float Time::previousTime = 0.0f;
float Time::deltaTime = 1.0f;
float Time::frameTime = 1.0f / (float)FPS;
float Time::timePassed = 0.0f;

void Time::calculateCurrentTime() {
	currentTime = glfwGetTime();
}

void Time::calculateDeltaTime() {
	deltaTime = currentTime - previousTime;
}
