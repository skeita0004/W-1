#pragma once
#include "GameObject.hpp"
#include "Model.hpp"
#include "BoxCollider.hpp"
#include "SphereCollider.hpp"
#include "Horse.hpp"

// 障害物クラスを管理するクラス
class Obstacle : public GameObject
{
public:
	Obstacle(GameObject* parent, const char* name);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void SetPosition(XMFLOAT3 pos);
protected:
	void SetBoxCollider(XMFLOAT3 center, XMFLOAT3 size);

	int hModel_;

	BoxCollider* bCollider_;
	SphereCollider* sCollider_;

	Horse* horse_;

	bool isColl_;

	XMFLOAT3 basePos_;
	XMFLOAT3 size_;
};