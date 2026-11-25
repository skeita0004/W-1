#pragma once
#include "GameObject.hpp"
#include "BoxCollider.hpp"

class Horse : public GameObject
{
public:
	Horse(GameObject* parent);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;

	void Run();
	void Jump();
	void NockBack();

	void OnCollision(GameObject* pTarget) override;

	XMFLOAT3 GetPosition(){return transform_.position;}
private:
	int hModel_;
	int sModel_;
	float speed_;

	XMFLOAT3 colPos_;

	float velocity_;
	float gravity_;
	float jumpPower_;

	bool isHit_;
	XMFLOAT3 knockbackVelocity_;
	float knockbackTime_;

	bool isGround_;
};