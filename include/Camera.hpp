#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

namespace gps
{

    enum MOVE_DIRECTION
    {
        FORWARD,
        BACKWARD,
        RIGHT,
        LEFT
    };

    constexpr float YAW = -90.0f;
    constexpr float PITCH = 0.0f;
    constexpr float SPEED = 1.0f;
    constexpr float SENSITIVITY = 0.1f;
    constexpr float ZOOM = 45.0f;

    class Camera
    {

    public:
        // Camera constructor
        explicit Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 0.1f, 0.0f), float yaw = YAW, float pitch = PITCH);
        Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch);
        // return the view matrix, using the glm::lookAt() function
        [[nodiscard]] glm::mat4 getViewMatrix() const;
        // update the camera internal parameters following a camera move event
        void ProcessKeyboard(MOVE_DIRECTION direction, float speed);
        // update the camera internal parameters following a camera rotate event
        // yaw - camera rotation around the y-axis
        // pitch - camera rotation around the x-axis
        void ProcessMouseMovement(float xoffset, float yoffset, bool constantPitch = true);
        void ProcessMouseScroll(float yoffset);
        [[nodiscard]] glm::vec3 getCameraPosition() const;

    private:
        glm::vec3 Position{};
        glm::vec3 Front{};
        glm::vec3 Up{};
        glm::vec3 Right{};
        glm::vec3 WorldUp{};

        float Yaw;
        float Pitch;

        float MovementSpeed;
        float MouseSensitivity;
        float Zoom;

        void updateCameraVectors();
    };
}
