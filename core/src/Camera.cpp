#include "Camera.hpp"

CameraFPS::CameraFPS()
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f), pitch(0.0f), fov(45.0), aspectRatio(4.0f / 3.0f), nearPlane(0.1f), farPlane(100.0f)
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    worldUp = up;
    UpdateCameraVectors();
}

void CameraFPS::Init(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    UpdateCameraVectors();
}
void CameraFPS::SetPosition(const glm::vec3 &pos)
{
    position = pos;
}

void CameraFPS::SetRotation(float newYaw, float newPitch)
{
    yaw = newYaw;
    pitch = newPitch;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    UpdateCameraVectors();
}

void CameraFPS::Move(const glm::vec3 &offset)
{
    position += offset;
}

void CameraFPS::Rotate(float deltaYaw, float deltaPitch)
{
    yaw += deltaYaw;
    pitch += deltaPitch;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    UpdateCameraVectors();
}

glm::mat4 CameraFPS::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 CameraFPS::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void CameraFPS::UpdateCameraVectors()
{
    // fps
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);
    right = glm::normalize(glm::cross(front, worldUp));
    up = glm::normalize(glm::cross(right, front));
}




Camera::Camera()
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)),
      yaw(-90.0f), pitch(0.0f), roll(90.0f), fov(45.0), aspectRatio( 4.0f / 3.0f), nearPlane( 0.1f), farPlane( 100.0f)
{
    position = glm::vec3(0.0f, 0.0f, 3.0f);
    worldUp = up;
    UpdateCameraVectors();
}



void Camera::Init(float fov, float aspectRatio, float nearPlane, float farPlane)
{
    this->fov = fov;
    this->aspectRatio = aspectRatio;
    this->nearPlane = nearPlane;
    this->farPlane = farPlane;

    UpdateCameraVectors();
}

void Camera::SetPosition(const glm::vec3 &pos)
{
    position = pos;
}

void Camera::SetRotation(float newYaw, float newPitch)
{
    yaw = newYaw;
    pitch = newPitch;
    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;
    UpdateCameraVectors();
}

void Camera::SetRoll(float newRoll)
{
    roll = newRoll;
    UpdateCameraVectors();
}

void Camera::MoveFront(float velocity)
{
    position += front * velocity;
    
}

void Camera::MoveBack(float velocity)
{
    position  -= front * velocity;
}

void Camera::StrafeRight(float velocity)
{
    position += right * velocity;
}

void Camera::StrafeLeft(float velocity)
{
    position  -= right * velocity;
}


void Camera::Rotate(float deltaYaw, float deltaPitch, float deltaRoll)
{
    yaw += deltaYaw;
    pitch += deltaPitch;
    roll += deltaRoll;
    UpdateCameraVectors();
}
void Camera::MouseView(float xoffset, float yoffset, bool constrainPitch)
{
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        yaw   += xoffset;
        pitch += yoffset;
    if (constrainPitch)
    {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
    return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::GetProjectionMatrix() const
{
    return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane);
}

void Camera::UpdateCameraVectors()
{
        glm::vec3 _front;
        glm::vec3 WorldUp(0.0f, 1.0f, 0.0f);
        _front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        _front.y = sin(glm::radians(pitch));
        _front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        front = glm::normalize(_front);
        // also re-calculate the Right and Up vector
        right = glm::normalize(glm::cross(front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        up    = glm::normalize(glm::cross(right, front));
}