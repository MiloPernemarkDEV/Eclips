#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    Camera();

    void setPosition(const glm::vec3& pos);
    void setTarget(const glm::vec3& target);
    void setUp(const glm::vec3& up);
    void update(GLFWwindow* window, float deltaTime);

    glm::mat4 getViewMatrix() const;

    // optional helpers (useful later for FPS camera)
    void move(const glm::vec3& delta);

private:
    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;

    float yaw = -90.0f;
    float pitch = 0.0f;

    float lastX = 0.0f;
    float lastY = 0.0f;

    bool firstMouse = true;

    float sensitivity = 0.1f;
};