#include "GameObject.hpp"
#include <cassert>
#include "SafeCleaning.hpp"
#include "Direct3D.hpp"

using namespace SafeCleaning;

GameObject::GameObject(GameObject * parent, const std::string& name)
	: pParent_(parent),

	objectName(name)
{
	childList_.clear();
	state_ = { 0, 1, 1, 0 };

	if(parent)
		transform.pParent = &parent->transform;

}

//デストラクタ
GameObject::~GameObject()
{
    using namespace SafeCleaning;

	for (auto it = colliderList.begin(); it != colliderList.end(); it++)
	{
		SafeDelete(*it);
	}
	colliderList.clear();
}

// 削除するかどうか
bool GameObject::IsDelete()
{
	return (state_.dead != 0);
}

// 自分を削除する
void GameObject::DeleteMe()
{
	state_.dead = 1;
}

// Updateを許可
void GameObject::EnableUpdate()
{
	state_.entered = 1;
}

// Updateを拒否
void GameObject::DisableUpdate()
{
	state_.entered = 0;
}

// Drawを許可
void GameObject::EnableDraw()
{
	state_.visible = 1;
}

// Drawを拒否
void GameObject::DisableDraw()
{
	state_.visible = 0;
}

// 初期化済みかどうか
bool GameObject::IsFinishInit()
{
	return (state_.initialized != 0);
}

// 初期化済みにする
void GameObject::CalledInit()
{
	state_.initialized = 1;
}

// Update実行していいか
bool GameObject::IsEnableUpdate()
{
	return (state_.entered != 0);
}

// Draw実行していいか
bool GameObject::IsEnableDraw()
{
	return (state_.visible != 0);
}

//子オブジェクトリストを取得
std::list<GameObject*>* GameObject::GetChildList()
{
	return &childList_;
}

//親オブジェクトを取得
GameObject * GameObject::GetParent(void)
{
	return pParent_;
}

Transform* GameObject::GetTransform()
{
	return &transform;
}

//名前でオブジェクトを検索（対象は自分の子供以下）
GameObject * GameObject::FindChildObject(const std::string & name)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return nullptr;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトから探す
	while (it != end) {
		//同じ名前のオブジェクトを見つけたらそれを返す
		if ((*it)->GetObjectName() == name)
			return *it;

		//その子供（孫）以降にいないか探す
		GameObject* obj = (*it)->FindChildObject(name);
		if (obj != nullptr)
		{
			return obj;
		}

		//次の子へ
		it++;
	}

	//見つからなかった
	return nullptr;
}



void GameObject::SetObjectName(const std::string& _name)
{
    objectName = _name;
}

//オブジェクトの名前を取得
const std::string& GameObject::GetObjectName(void) const
{
	return objectName;
}

//子オブジェクトを追加（リストの最後へ）
void GameObject::PushBackChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_back(obj);
}

//子オブジェクトを追加（リストの先頭へ）
void GameObject::PushFrontChild(GameObject * obj)
{
	assert(obj != nullptr);

	obj->pParent_ = this;
	childList_.push_front(obj);
}

//子オブジェクトを全て削除
void GameObject::DeleteAllChildren(void)
{
	//子供がいないなら終わり
	if (childList_.empty())
		return;

	//イテレータ
	auto it = childList_.begin();	//先頭
	auto end = childList_.end();	//末尾

	//子オブジェクトを1個ずつ削除
	while (it != end)
	{
		KillObjectSub(*it);
		delete *it;
		it = childList_.erase(it);
	}

	//リストをクリア
	childList_.clear();
}

//オブジェクト削除（再帰）
void GameObject::KillObjectSub(GameObject * obj)
{
	if (!childList_.empty())
	{
		auto list = obj->GetChildList();
		auto it = list->begin();
		auto end = list->end();
		while (it != end)
		{
			KillObjectSub(*it);
			delete *it;
			it = list->erase(it);
		}
		list->clear();
	}
	obj->Release();
}



//コライダー（衝突判定）を追加する
void GameObject::AddCollider(Collider* collider)
{
	collider->SetGameObject(this);
	colliderList.push_back(collider);
}

//コライダー（衝突判定）を削除
void GameObject::DeleteCollider()
{
	for (auto it = colliderList.begin(); it != colliderList.end(); it++)
	{
		SafeDelete(*it);
	}
	colliderList.clear();
}

//衝突判定
void GameObject::Collision(GameObject * pTarget)
{
	//自分同士の当たり判定はしない
	if (pTarget == nullptr || this == pTarget)
	{
		return;
	}

	//自分とpTargetのコリジョン情報を使って当たり判定
	//1つのオブジェクトが複数のコリジョン情報を持ってる場合もあるので二重ループ
	for (auto i = this->colliderList.begin(); i != this->colliderList.end(); i++)
	{
		for (auto j = pTarget->colliderList.begin(); j != pTarget->colliderList.end(); j++)
		{
			if ((*i)->IsHit(*j))
			{
				//当たった
				this->OnCollision(pTarget);
			}
		}
	}

	//子供がいないなら終わり
	if (pTarget->childList_.empty())
		return;

	//子供も当たり判定
	for (auto i = pTarget->childList_.begin(); i != pTarget->childList_.end(); i++)
	{
		Collision(*i);
	}
}


//テスト用の衝突判定枠を表示
void GameObject::CollisionDraw()
{
	Direct3D::SetShader(Direct3D::SHADER_UNLIT);

	for (auto i = this->colliderList.begin(); i != this->colliderList.end(); i++)
	{
		(*i)->Draw(GetTransform()->position);
	}

	Direct3D::SetShader(Direct3D::SHADER_3D);
}

//RootJobを取得
GameObject * GameObject::GetRootJob()
{
	if (GetParent() == nullptr)
	{
		return this;
	}
	else return GetParent()->GetRootJob();
}




void GameObject::UpdateSub()
{
	Update();
	//Transform();

	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
        if ((*it)->IsEnableUpdate())
        {
		    (*it)->UpdateSub();
        }
	}

	for (auto it = childList_.begin(); it != childList_.end();)
	{
		if ((*it)->IsDelete() == true)
		{
			(*it)->ReleaseSub();
			SafeDelete(*it);
			it = childList_.erase(it);
		}
		else
		{
			//当たり判定
			(*it)->Collision(GetParent());
			it++;
		}
	}
}

void GameObject::DrawSub()
{
    if (IsEnableDraw())
    {
	    Draw();
    }


	//リリース時は削除
#ifdef _DEBUG
		//コリジョンの描画
	if (Direct3D::isDrawCollision_)
	{
		CollisionDraw();
	}
#endif

	//その子オブジェクトの描画処理
	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
        if ((*it)->IsEnableDraw())
        {
		    (*it)->DrawSub();
        }
	}
}

void GameObject::ReleaseSub()
{
	//コライダーを削除
	DeleteCollider();


	for (auto it = childList_.begin(); it != childList_.end(); it++)
	{
		(*it)->ReleaseSub();
		SafeDelete(*it);
	}

	Release();
}

//ワールド行列の取得（親の影響を受けた最終的な行列）
XMMATRIX GameObject::GetWorldMatrix(void)
{
	return transform.GetWorldMatrix();
}


