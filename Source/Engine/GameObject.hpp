#pragma once
#include <DirectXMath.h>
#include <list>
#include <string>
#include <cassert>
#include "SphereCollider.hpp"
#include "BoxCollider.hpp"
#include "Transform.hpp"
#include <vector>

using namespace DirectX;

/**
* @brief 全てのゲームオブジェクト（シーンも含めて）が継承するインターフェース
* @detail ゲームオブジェクトは、親子構造になっていて、
*         マトリクスの影響を受けることになる
*/
class GameObject
{
public:

	/// @brief コンストラクタ
	/// @param _pParent 親
	/// @param _name 名前
    /// @note  親というのは、親クラスのことではなく、
    ///        座標変換において、親子関係を持つ物のことを指す
	GameObject(GameObject* _pParent, const std::string& _name);

	virtual ~GameObject();

	/// @brief 初期化処理
	virtual void Init(void) = 0;

	/// @brief 更新処理
	virtual void Update(void) = 0;

	/// @brief 描画処理
	virtual void Draw() = 0;

	/// @brief 解放処理
	virtual void Release(void) = 0;

	//自分の該当関数を読んだ後、子供の関数も呼ぶ
	void UpdateSub();
	void DrawSub();
	void ReleaseSub();

	//ワールド行列の取得（親の影響を受けた最終的な行列）
	//戻値：ワールド行列
    // これはダメだと思うわ、
    // 座標系の処理はtransformにいるべきよ。
    // (西麻布のママより)
	XMMATRIX GetWorldMatrix();

	//---各フラグの制御---//
	/// @brief  オブジェクトがDeleteMeを呼んだのかを返す
    /// @detail このフラグがtrueのオブジェクトは削除される。
	/// @return true: 呼んだ, false: 呼んでいない
	bool IsDelete();

	/// @brief  自分を消す
    /// @detail オブジェクトのリストから削除される
	void DeleteMe();

	void EnableUpdate();
	void DisableUpdate();

    void EnableDraw();
	void DisableDraw();

    /// @brief 初期化を終えたかどうか
    /// @return true: Initが呼ばれた
    bool IsFinishInit();

	/// @brief 初期化済みにする
	void CalledInit();

    bool IsEnableUpdate();
	bool IsEnableDraw();

	std::list<GameObject*>* GetChildList();

	GameObject* GetParent();

    Transform* GetTransform();

	//名前でオブジェクトを検索（対象は自分の子供以下）
	//引数：name	検索する名前
	//戻値：見つけたオブジェクトのアドレス（見つからなければnullptr）
	GameObject* FindChildObject(const std::string& name);

	//名前でオブジェクトを検索（対象は全体）
	//引数：検索する名前
	//戻値：見つけたオブジェクトのアドレス
    template <typename T>
	inline T* FindObject(const std::string& _name)
    { 
        return static_cast<T*>(GetRootJob()->FindChildObject(_name));
    }


    void  SetObjectName(const std::string& _name);
	const std::string& GetObjectName(void) const;

	void PushBackChild(GameObject* obj);
	void PushFrontChild(GameObject* obj);

	void DeleteAllChildren();

	//コライダー（衝突判定）を追加する
	void AddCollider(Collider * collider);

    XMFLOAT3 GetBoxColliderSize(int _index = 0)
    {
        return colliderList[_index]->GetBoxSize();
    }

	//何かと衝突した場合に呼ばれる（オーバーライド用）
	//引数：pTarget	衝突した相手
	virtual void OnCollision(GameObject* pTarget) {};

	void DeleteCollider();

	//衝突判定
	//引数：pTarget	衝突してるか調べる相手
	void Collision(GameObject* pTarget);

	//テスト用の衝突判定枠を表示
	void CollisionDraw();

	GameObject* GetRootJob();

protected:
	
	//位置や向きなどを管理するオブジェクト
	Transform				transform;

	//オブジェクトの名前
	std::string				objectName;

	//衝突判定リスト
	std::vector<Collider*>	colliderList;	

    /// @brief ゲームオブジェクトの子クラスをインスタンスするテンプレート
    /// @param T ゲームオブジェクトの子クラス型
    /// @param _pParent インスタンス対象クラスの親（継承元のことではない）
    /// @return 実体へのポインタ(型は、実体の型)
    template <class T>
    GameObject* Instantiate(GameObject* _pParent)
    {
	    T* pNewObject = new T(_pParent);
	    if (_pParent != nullptr)
	    {
		    _pParent->PushBackChild(pNewObject);
	    }
	    pNewObject->Init();
        pNewObject->CalledInit();
	    return pNewObject;
    }

private:

	//オブジェクト削除（再帰）
	//引数：obj　削除するオブジェクト
	void KillObjectSub(GameObject* obj);

private:
	//フラグ(bitfield)
	struct OBJECT_STATE
	{
		unsigned initialized : 1;	//初期化済みか
		unsigned entered : 1;		//更新するか
		unsigned visible : 1;		//描画するか
		unsigned dead : 1;			//削除するか
	} state_;

	//親オブジェクト
	GameObject* pParent_;

	//子オブジェクトリスト
	std::list<GameObject*> childList_;
};

