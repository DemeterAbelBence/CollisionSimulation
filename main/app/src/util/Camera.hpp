#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

#include "gl/GpuProgram.hpp"
#include "math/Quaternion.hpp"
#include "glfw/Time.hpp"

class Camera {

private:
	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;
	glm::vec3 direction;

	glm::mat4 projection;
	glm::mat4 view;

private:
	float near;
	float far;
	float top;
	float bottom;
	float left;
	float right;
	float field_of_view;

private:
	float move_speed;
	float rotate_speed;

private:
	glm::mat4 createOrthographic();
	glm::mat4 createPerspective();
	glm::mat4 createViewMatrix();

public:
	Camera();
	Camera(glm::vec3 e, glm::vec3 t, glm::vec3 u);

	void setNear(float value) { near = value; }
	void setFar(float value) { far= value; }
	void setTop(float value) { top = value; }
	void setBottom(float value) { bottom = value; }
	void setLeft(float value) { left = value; }
	void setRight(float value) { right = value; }
	void setFieldOfView(float value) { field_of_view = value; }
	void setEye(glm::vec3 value) { eye = value; }
	void setTarget(glm::vec3 value) { target = value; }
	void setUp(glm::vec3 value) { up = value; }

	void setMoveSpeed(float value) { move_speed = value; }
	void setRotateSpeed(float value) { rotate_speed = value; }

	glm::vec3 getEye() const { return eye; }
	glm::mat4 getProjection() const { return projection; }
	glm::mat4 getView() const { return view; }

public:
	void resetProjection() { projection = createPerspective(); }
	void resetView() { view = createViewMatrix(); }

	void moveForward(int sign);
	void moveRight(int sign);
	void moveUp(int sign);
	void rotate(int sign);

	void setUniforms(const GpuProgram& program) const;
};