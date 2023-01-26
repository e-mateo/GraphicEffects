#include <Camera.h>


LowRenderer::Camera::Camera(const float width, const float height)
{
    // width and height are useful to compute projection matrix with the right aspect ratio
    aspectRatio = width / height;
    position = { 0,0,0 };
    angleTheta = 180.f;
    anglePhi = 0.f;
    viewMatrix = maths::Mat4::identity();
    projectionMatrix = GetProjection(90.f, 0.05f, 10000.f);

    moveCamera = false;
    isLocked = false;
    isFocused = false;
    canUnlock = false;
}

void LowRenderer::Camera::Update(const double p_deltaTime, bool moveCamera, const LowRenderer::CameraInputs& inputs, int typeCollider, maths::Vec3 posPlayer, maths::Vec3 scalePlayer)
{
    if (moveCamera)
    {
        if(!inputs.invertXAxis)
            angleTheta += (inputs.mouseDeltaX / 150.f) * p_deltaTime;
        else
            angleTheta -= (inputs.mouseDeltaX / 150.f) * p_deltaTime;

        if(inputs.invertYAxis)
            anglePhi += (inputs.mouseDeltaY / 150.f) * p_deltaTime;
        else
            anglePhi -= (inputs.mouseDeltaY / 150.f) * p_deltaTime;
    }

    maths::Vec4 dirCamVec4(0, 0, -1, 1);
    dirCamVec4 = maths::Mat4::CreateYRotationMatrix(angleTheta) * maths::Mat4::CreateXRotationMatrix(anglePhi) * dirCamVec4;
    maths::Vec3 dirCam = maths::Vec3(dirCamVec4.x, dirCamVec4.y, dirCamVec4.z);

    maths::Vec4 dirXVec4(1, 0, 0, 1);
    dirXVec4 = maths::Mat4::CreateYRotationMatrix(angleTheta) * dirXVec4;
    dirX = maths::Vec3(dirXVec4.x, dirXVec4.y, dirXVec4.z);

    maths::Vec4 dirZVec4(0, 0, -1, 1);
    dirZVec4 = maths::Mat4::CreateYRotationMatrix(angleTheta) * dirZVec4;
    dirZ = maths::Vec3(dirZVec4.x, dirZVec4.y, dirZVec4.z);

    position = posPlayer + maths::Vec3(0, 30, 0);

    viewMatrix = GetViewMatrix();
    viewMatrix.FillArray();
    viewProjectionMatrix = projectionMatrix * viewMatrix;
}

maths::Mat4 LowRenderer::Camera::GetViewMatrix()
{
    maths::Mat4 cameraMat = maths::Mat4::CreateTransformMatrix(maths::Vec3(anglePhi, angleTheta, 0), maths::Vec3(position.x, position.y, position.z), maths::Vec3(1, 1, 1));
    return cameraMat.inverseMatrix();
}

maths::Mat4 LowRenderer::Camera::GetProjection(const float fov, const float near, const float far)
{
    float top = tanf(fov * maths::PI / (2 * 180)) * near;
    float bottom = -top;
    float right = top * aspectRatio;
    float left = -top * aspectRatio;

    float tab[4][4] = {
        {(2 * near) / (right - left), 0.f, (right + left) / (right - left),0.f },
        {0.f,(2 * near) / (top - bottom),(top + bottom) / (top - bottom), 0.f  },
        {0.f,0.f,-(far + near) / (far - near), -(2 * far * near) / (far - near)},
        {0.f,0.f,-1.f,0.f},
    };
    maths::Mat4 Proj(tab);
    Proj.FillArray();

    return Proj;
}