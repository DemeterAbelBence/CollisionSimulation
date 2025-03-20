#include "glfw.hpp"

glfw::Key::Key(unsigned int _id) {
    id = _id;
    pressed = false;
}

bool glfw::Key::isPressed(GLFWwindow* window) {
    if (getState(window) == GLFW_PRESS) {
        pressed = true;
        return true;
    }
    return false;
}

bool glfw::Key::isReleased(GLFWwindow* window) {
    if (getState(window) == GLFW_RELEASE && pressed) {
        pressed = false;
        return true;
    }
    else return false;
}

unsigned int glfw::Key::getState(GLFWwindow* window) {
    return glfwGetKey(window, id);
}
