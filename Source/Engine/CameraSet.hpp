#pragma once
#include <vector>
//#include <string>

#include <DirectXMath.h>

#include "IVirtualCamera.hpp"
#include "CameraType.hpp"

class Transform;

namespace CameraSet
{
    void Initialize();
    void Release();

    /// @brief              透視投影カメラを作成する
    /// @param _type        カメラのタイプ
    /// @param _upDir       カメラの絶対的な上の方向
    /// @param _fovAngleDeg 視野角(度数法の値で指定)
    /// @param _nearClip    最近クリッピング距離
    /// @param _farClip     最遠クリッピング距離
    /// @return             カメラのハンドル
    int Create(const CameraType _type,
               const XMFLOAT3&  _upDir,
               const float      _fovAngleDeg,
               const float      _nearClip,
               const float      _farClip);

    /// @brief           並行投影カメラを作成する
    /// @param _type     カメラのタイプ
    /// @param _upDir    カメラの絶対的な上の方向
    /// @param _viewSize 投影面のサイズ(x, y)
    /// @param _nearClip 最近クリッピング距離
    /// @param _farClip  最遠クリッピング距離
    /// @return          カメラのハンドル
    int CreateOrtho(const CameraType _type,
                    const XMFLOAT3&  _upDir,
                    const XMFLOAT2&  _viewSize,
                    const float      _nearClip,
                    const float      _farClip);

    /// @brief Currentとして設定されている
    /// カメラのTransformを返す
    /// @return カメラのTransform
    Transform& GetTransform();

    /// @brief 指定したカメラをCurrentにする
    /// @param _hCam カメラのハンドル
    void SetCurrent(int _hCam);

    /// @brief Currentとして設定されているカメラの
    /// ポインタを返す
    /// @return IVirtualCamera*
    IVirtualCamera* GetCurrent();

    /// @brief _handleに対応するカメラのポインタを返す
    /// @param _handle カメラのハンドル
    /// @return IVirtualCamera*
    IVirtualCamera* GetVirtualCamera(int _handle);

    // void FindCamera(CameraType _type);

    // デフォルトの透視投影レンダリング用カメラ
    inline int hNormalCamera{0};

    // デフォルトのシャドウマップ用並行投影カメラ
    inline int hShadowCamera{0};

    // デフォルトの平面反射用透視投影カメラ
    inline int hReflectionCamera{0};
};