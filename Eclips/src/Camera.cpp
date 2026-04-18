#include "pch.h"
#include "Camera.h"

Camera::Camera()
    : position(2.0f, -10.0f, 2.0f),
    target(0.0f, 0.0f, 0.0f),
    up(0.0f, 0.0f, 1.0f)
{
}

void Camera::setPosition(const glm::vec3& pos)
{
    position = pos;
}

void Camera::setTarget(const glm::vec3& t)
{
    target = t;
}

void Camera::setUp(const glm::vec3& u)
{
    up = u;
}

void Camera::update(GLFWwindow* window, float deltaTime)
{
    float speed = 30.0f * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        move(glm::vec3(0.0f, speed, 0.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        move(glm::vec3(0.0f, -speed, 0.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        move(glm::vec3(-speed, 0.0f, 0.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        move(glm::vec3(speed, 0.0f, 0.0f));
    }

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        move(glm::vec3(0.0f, 0.0f, speed));
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        move(glm::vec3(0.0f, 0.0f, -speed));
    }
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(position, target, up);
}

void Camera::move(const glm::vec3& delta)
{
    position += delta;
    target += delta; // keeps camera locked on same relative focus
}