#pragma once

#include <DirectXMath.h>
#include "CameraSet.hpp"

class DebugCamera
{
public:
    void Initialize();
    void Enable();
    void Disable();
    void Update();

    bool IsEnabled() const
    {
        return enabled_;
    }

private:
    void UpdateViewDirection();

    bool enabled_ = false;

    //初期カメラ状態
    DirectX::XMFLOAT3 originalPosition_{};
    DirectX::XMFLOAT3 originalTargetPosition_{};
    DirectX::XMFLOAT3 originalTargetRotate_{};

    //操作用
    float yaw_ = 0.0f;
    float pitch_ = 0.0f;

    float moveSpeed_ = 5.0f;
    float rotateSpeed_ = 60.0f;

    int cameraHandle_ = -1;
};