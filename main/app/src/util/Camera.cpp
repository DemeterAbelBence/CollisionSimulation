#include "Camera.hpp"

glm::mat4 Camera::createOrthographic() {
    float x1 = 2.0f / (right - left);
    float x4 = -(right + left) / (right - left);
    float y2 = 2.0f / (top - bottom);
    float y4 = -(top + bottom) / (top - bottom);
    float z3 = -2.0f / (far - near);
    float z4 = -(far + near) / (far - near);

    glm::mat4 orthographic;
    orthographic[0] = glm::vec4(x1, 0.0f, 0.0f, x4);
    orthographic[1] = glm::vec4(0.0f, y2, 0.0f, y4);
    orthographic[2] = glm::vec4(0.0f, 0.0f, z3, z4);
    orthographic[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return orthographic;
}

glm::mat4 Camera::createPerspective() {
    float z3 = -(far + near) / (far - near);
    float z4 = -2.0f * far * near / (far - near);
    float angle = 3.14159f * field_of_view / 2;
    float fov = glm::tan(angle);
    float scale = 1.0f / (fov != 3.14159f / 2 ? fov : 1.0f);

    glm::mat4 perspective;
    perspective[0] = glm::vec4(scale, 0.0f, 0.0f, 0.0f);
    perspective[1] = glm::vec4(0.0f, scale, 0.0f, 0.0f);
    perspective[2] = glm::vec4(0.0f, 0.0f, z3, z4);
    perspective[3] = glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);

    return perspective;
}

glm::mat4 Camera::createViewMatrix() {
    glm::vec3 dir = glm::normalize(direction);
    glm::vec3 right = glm::normalize(glm::cross(up, dir));
    glm::vec3 up = glm::normalize(glm::cross(dir, right));

    glm::mat4 rotation;
    rotation[0] = glm::vec4(right, 0.0f);
    rotation[1] = glm::vec4(up, 0.0f);
    rotation[2] = glm::vec4(dir, 0.0f);
    rotation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    glm::mat4 translation;
    translation[0] = glm::vec4(1.0f, 0.0f, 0.0f, -eye.x);
    translation[1] = glm::vec4(0.0f, 1.0f, 0.0f, -eye.y);
    translation[2] = glm::vec4(0.0f, 0.0f, 1.0f, -eye.z);
    translation[3] = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

    return translation * rotation;
}

Camera::Camera() {
    eye = glm::vec3(0.0f, 0.0f, 20.0f);
    target = glm::vec3(0.0f, 0.0f, 0.0f);
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    direction = glm::normalize(eye - target);

    field_of_view = 0.5f;
    near = 0.1f;
    far = 200.0f;
    top = 1.0f;
    bottom = -1.0f;
    right = 1.0f;
    left = -1.0f;

    resetProjection();
    resetView();
}

Camera::Camera(glm::vec3 e, glm::vec3 t, glm::vec3 u) {
    eye = e;
    target = t;
    up = glm::normalize(u);
    direction = glm::normalize(eye - target);

    resetProjection();
    resetView();
}

void Camera::moveForward(int sign) {
    eye += direction * (Time::deltaTime * move_speed * sign);
}

void Camera::moveRight(int sign) {
    glm::vec3 dir = glm::normalize(direction);
    glm::vec3 right = glm::normalize(glm::cross(up, dir));

    eye += right * (Time::deltaTime * move_speed * sign);
}

void Camera::moveUp(int sign) {
    glm::vec3 dir = glm::normalize(direction);
    glm::vec3 right = glm::normalize(glm::cross(up, dir));
    glm::vec3 up = glm::normalize(glm::cross(dir, right));

    eye += up * (Time::deltaTime * move_speed * sign);
}

void Camera::rotate(int sign) {
    float rotation = Time::deltaTime * rotate_speed * sign;
    Quaternion r = Quaternion(glm::vec3(0.0f, 1.0f, 0.0f), rotation).normalize();
    Quaternion rc = r.conjugate().normalize();
    Quaternion d = Quaternion(direction);
    d = (r * d * rc).normalize();
    direction = glm::vec3(d.x, d.y, d.z);
}

void Camera::setUniforms(const GpuProgram& program) const{
    program.bind();
    program.setMat4("P", projection);
    program.setMat4("V", view);
    program.setVec3("eye", eye);
}   


