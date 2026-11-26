#include "DebugCamera.hpp"
#include "Input.hpp"

#include <cmath>

using namespace DirectX;

void DebugCamera::Initialize()
{
    auto camera = CameraSet::GetCurrent();

    originalPosition_ = camera->GetTransform().position;
    originalTargetPosition_ = camera->GetTargetTransform().position;
    originalTargetRotate_ = camera->GetTargetTransform().rotate;

    yaw_ = originalTargetRotate_.y;
    pitch_ = originalTargetRotate_.x;
}

void DebugCamera::Enable()
{
    if (enabled_)
        return;

    auto camera = CameraSet::GetCurrent();

    //現在のカメラ状態を保存
    originalPosition_ = camera->GetTransform().position;
    originalTargetPosition_ = camera->GetTargetTransform().position;
    originalTargetRotate_ = camera->GetTargetTransform().rotate;

    yaw_ = originalTargetRotate_.y;
    pitch_ = originalTargetRotate_.x;

    enabled_ = true;
}

void DebugCamera::Disable()
{
    if (!enabled_)
        return;

    auto camera = CameraSet::GetCurrent();

    //カメラを元の状態へ戻す
    camera->GetTransform().position = originalPosition_;
    camera->GetTargetTransform().position = originalTargetPosition_;
    camera->GetTargetTransform().rotate = originalTargetRotate_;

    enabled_ = false;
}

void DebugCamera::Update()
{
    if (!enabled_)
        return;

    auto camera = CameraSet::GetCurrent();

    auto& camTransform = camera->GetTransform();
    auto& targetTransform = camera->GetTargetTransform();

    constexpr float MOVE_SPEED = 5.0f;
    constexpr float ROTATE_SPEED = 60.0f;
    constexpr float DELTA_TIME = 1.0f / 60.0f;

    const float moveAmount = MOVE_SPEED * DELTA_TIME;
    const float rotateAmount = ROTATE_SPEED * DELTA_TIME;

    const bool shift =
        Input::IsKey(DIK_LSHIFT) ||
        Input::IsKey(DIK_RSHIFT);

    //視点回転
    if (Input::IsKey(DIK_Q) || Input::IsKey(DIK_LEFT))
        yaw_ -= rotateAmount;

    if (Input::IsKey(DIK_E) || Input::IsKey(DIK_RIGHT))
        yaw_ += rotateAmount;

    //上下視点回転
    if (Input::IsKey(DIK_UP))
        pitch_ -= rotateAmount;

    if (Input::IsKey(DIK_DOWN))
        pitch_ += rotateAmount;

    //上下視点の制限
    pitch_ = (pitch_ < -89.0f) ? -89.0f :
        (pitch_ > 89.0f) ? 89.0f : pitch_;

    //回転角を反映
    targetTransform.rotate.x = pitch_;
    targetTransform.rotate.y = yaw_;
    targetTransform.rotate.z = 0.0f;

    //視点方向を計算
    const float yawRad = XMConvertToRadians(yaw_);
    const float pitchRad = XMConvertToRadians(pitch_);

    const float forwardX = std::sin(yawRad) * std::cos(pitchRad);
    const float forwardY = -std::sin(pitchRad);
    const float forwardZ = std::cos(yawRad) * std::cos(pitchRad);

    //右方向を計算
    const float rightX = std::cos(yawRad);
    const float rightZ = -std::sin(yawRad);

    //前後移動
    if (Input::IsKey(DIK_W))
    {
        if (shift)
        {
            camTransform.position.y += moveAmount;
        }
        else
        {
            camTransform.position.x += forwardX * moveAmount;
            camTransform.position.y += forwardY * moveAmount;
            camTransform.position.z += forwardZ * moveAmount;
        }
    }

    if (Input::IsKey(DIK_S))
    {
        if (shift)
        {
            camTransform.position.y -= moveAmount;
        }
        else
        {
            camTransform.position.x -= forwardX * moveAmount;
            camTransform.position.y -= forwardY * moveAmount;
            camTransform.position.z -= forwardZ * moveAmount;
        }
    }

    //左右移動
    if (Input::IsKey(DIK_A))
    {
        camTransform.position.x -= rightX * moveAmount;
        camTransform.position.z -= rightZ * moveAmount;
    }

    if (Input::IsKey(DIK_D))
    {
        camTransform.position.x += rightX * moveAmount;
        camTransform.position.z += rightZ * moveAmount;
    }

    //視点方向を再設定
    targetTransform.position = XMFLOAT3(0.0f, 0.0f, 1.0f);
}