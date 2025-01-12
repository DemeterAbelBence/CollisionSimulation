#pragma once

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>

#include "gl/GpuProgram.hpp"
#include "datatypes/Quaternion.hpp"
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

	inline void setNear(float value) { near = value; }
	inline void setFar(float value) { far= value; }
	inline void setTop(float value) { top = value; }
	inline void setBottom(float value) { bottom = value; }
	inline void setLeft(float value) { left = value; }
	inline void setRight(float value) { right = value; }
	inline void setFieldOfView(float value) { field_of_view = value; }
	inline void setEye(glm::vec3 value) { eye = value; }
	inline void setTarget(glm::vec3 value) { target = value; }
	inline void setUp(glm::vec3 value) { up = value; }

	inline void setMoveSpeed(float value) { move_speed = value; }
	inline void setRotateSpeed(float value) { rotate_speed = value; }

	inline glm::vec3 getEye() const { return eye; }
	inline glm::mat4 getProjection() const { return projection; }
	inline glm::mat4 getView() const { return view; }

public:
	inline void resetProjection() { projection = createPerspective(); }
	inline void resetView() { view = createViewMatrix(); }

	void moveForward(int sign);
	void moveRight(int sign);
	void moveUp(int sign);
	void rotate(int sign);

	void setUniforms(const GpuProgram& program) const;
};