#include "CameraSet.hpp"
#include "VirtualCameraPersp.hpp"
#include "VirtualCameraOrtho.hpp"
#include "SafeCleaning.hpp"

CameraSet::CameraSet()
{
}

void CameraSet::Init()
{
    instances_.push_back(new VirtualCameraPersp(XMFLOAT3(0, 1, 0),
                                                120.f,
                                                0.1f,
                                                1000.f));
    instances_[hCurrent_]->Init();
}

void CameraSet::Release()
{
    for (auto& instance : instances_)
    {
        SafeCleaning::SafeDelete(instance);
    }
    instances_.clear();
}

int CameraSet::Create(const DirectX::XMFLOAT3& _upDir,
                      const float _fovAngleDeg,
                      const float _nearClip,
                      const float _farClip)
{
    instances_.push_back(new VirtualCameraPersp(_upDir,
                                                _fovAngleDeg,
                                                _nearClip,
                                                _farClip));
    int handle = instances_.size() - 1;
    instances_[handle]->Init();

    return handle;
}

int CameraSet::CreateOrtho(const DirectX::XMFLOAT3 &_upDir,
                           const XMFLOAT2 _viewAngle,
                           const float _nearClip,
                           const float _farClip)
{
    instances_.push_back(new VirtualCameraOrtho(_upDir,
                                                _viewAngle,
                                                _nearClip,
                                                _farClip));
    int handle = instances_.size() - 1;
    instances_[handle]->Init();

    return handle;
}

Transform& CameraSet::GetTransform()
{
    return instances_[hCurrent_]->GetTransform();
}

void CameraSet::SetCurrent(int _hCam)
{
    hCurrent_ = _hCam;
}

IVirtualCamera* CameraSet::GetCurrent()
{
    return instances_[hCurrent_];
}
