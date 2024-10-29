#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class CameraFPS
{
public:
    CameraFPS();

    void Init(float fov, float aspectRatio, float nearPlane, float farPlane);


    void SetPosition(const glm::vec3 &position);
    void SetRotation(float yaw, float pitch);

  
    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

 
    void Move(const glm::vec3 &offset);
    void Rotate(float deltaYaw, float deltaPitch);

  
    glm::vec3 GetPosition() const { return position; }
    glm::vec3 GetFront() const { return front; }

private:
    void UpdateCameraVectors();


    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;


    float yaw;
    float pitch;

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
};



class Camera {
public:
    Camera();
    void Init(float fov, float aspectRatio, float nearPlane, float farPlane);

    void SetPosition(const glm::vec3& position);
    void SetRotation(float yaw, float pitch);
    void SetRoll(float roll);  

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;

    void MouseView(float xoffset, float yoffset, bool constrainPitch = true);

    void MoveFront(float velocity);
    void MoveBack(float velocity);
    void StrafeLeft(float velocity);
    void StrafeRight(float velocity);
    void Rotate(float deltaYaw, float deltaPitch, float deltaRoll=0.0); // Rotação em 6DOF

    glm::vec3 GetPosition() const { return position; }

private:
    void UpdateCameraVectors();

    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    float yaw;
    float pitch;
    float roll;  

    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;
    float MouseSensitivity = 0.5f;
};