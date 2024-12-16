#include "Camera.hpp"

namespace gps
{

    Camera::Camera(const glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        MouseSensitivity = SENSITIVITY;
        MovementSpeed = SPEED;
        Zoom = ZOOM;

        updateCameraVectors();
    }

    Camera::Camera(const float posX, const float posY, const float posZ, const float upX, const float upY, const float upZ, const float yaw, const float pitch)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        MouseSensitivity = SENSITIVITY;
        MovementSpeed = SPEED;
        Zoom = ZOOM;

        updateCameraVectors();
    }

    glm::mat4 Camera::getViewMatrix() const
    {
        return lookAt(Position, Position + Front, Up);
    }

    void Camera::ProcessKeyboard(const MOVE_DIRECTION direction, const float speed)
    {
        float velocity = MovementSpeed * speed;
        if (direction == FORWARD)
            Position += Front * velocity;
        if (direction == BACKWARD)
            Position -= Front * velocity;
        if (direction == LEFT)
            Position -= Right * velocity;
        if (direction == RIGHT)
            Position += Right * velocity;
    }

    void Camera::ProcessMouseMovement(float xoffset, float yoffset, const bool constantPitch)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw += xoffset;
        Pitch += yoffset;

        if (constantPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        updateCameraVectors();
    }

    void Camera::ProcessMouseScroll(const float yoffset)
    {
        Zoom -= static_cast<float>(yoffset);
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

    glm::vec3 Camera::getCameraPosition() const
    {
        return Position;
    }

    float Camera::getZoom() const
    {
        return Zoom;
    }

    void Camera::updateCameraVectors()
    {
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = normalize(front);
        Right = normalize(cross(Front, WorldUp));
        Up = normalize(cross(Right, Front));
    }

}