#pragma once
#include <iostream>

#include <Maths.h>


namespace LowRenderer
{
    struct CameraInputs
    {
        bool jump;
        bool moveLeft;
        bool moveRight;
        bool moveForward;
        bool moveBackward;
        bool invertXAxis = false;
        bool invertYAxis = false;

        char moveLeftInput[2] = "A";
        char moveRightInput[2] = "D";
        char moveForwardInput[2] = "W";
        char moveBackwardInput[2] = "S";

        bool mouseCaptured = false;

        float reduce = 100.f;
        float mouseDeltaX = 0.f;
        float mouseDeltaY = 0.f;

        double mouseX = 0.f;
        double mouseY = 0.f;
    };

    class Camera
    {
    public:
        Camera() {};
        Camera(const float p_width, const float p_height);

        void Update(const double p_deltaTime, bool moveCamera, const CameraInputs& p_inputs, int typeCollider, maths::Vec3 posPlayer, maths::Vec3 scalePlayer);

        maths::Mat4 GetViewMatrix();
        maths::Vec3 GetPosition() { return position; };
        maths::Mat4 GetVPMatrix() { return viewProjectionMatrix; };
        maths::Mat4 GetProjection(const float fov, const float near, const float far);

        inline maths::Mat4 GetProjMatrix() { return projectionMatrix; }

    private:
        float anglePhi = 0;
        float aspectRatio = 0;

        maths::Vec3 position;
        maths::Mat4 viewMatrix;
        maths::Mat4 projectionMatrix;
        maths::Mat4 viewProjectionMatrix;

    public:
        float angleTheta = 90.f;

        maths::Vec3 dirZ;
        maths::Vec3 dirX;

        bool isLocked;
        bool canUnlock;
        bool isFocused;
        bool moveCamera;
    };
}