#include "Bin.h"
#include "./Engine/Model.h"
#include "Engine/Input.h"
#include "Engine/Global.h"
#include "Beermug.h"
#include "WaterFlowOrigin.h"

#include <algorithm>
#include <cmath>

#include <imgui.h>

namespace
{
	const float IDLE_ANGLE_Z{ -20.f };
	const float POURING_ANGLE_Z{ -60.f };
}

Bin::Bin(GameObject *_pParent) :
	GameObject(_pParent, "Bin"),
	hModel_(-1),
	pouringTime_(),
	isPouring_(false),
	isConfirmed_(false),
	pBeermug_(nullptr),
	beermugInstanceRetrieved_(false)
{
}

void Bin::Initialize()
{
	binID_ = binCount_ + 1;

	hModel_ = Model::Load("Model/bin_mock.fbx");

	pWFO_ = Instantiate<WaterFlowOrigin>(this);
	XMFLOAT3 wfoPos = Model::GetBonePosition(hModel_, "BottleMouse");
	pWFO_->SetPosition(wfoPos);
	pWFO_->Invisible();

	transform_.position_.y = 0.6f;

	binCount_++;
}

void Bin::Update()
{

	if (not(beermugInstanceRetrieved_))
	{
		size_t pos = objectName_.size() - 1;
		pBeermug_ = static_cast<Beermug*>(FindObject("Beermug" + objectName_.substr(pos)));
		beermugInstanceRetrieved_ = true;
	}

	if (not(isConfirmed_))
	{
		if (isPouring_)
		{
			pouringTime_++;
			
			isPouring_ = true;
			static float bias{ 0.5 };
			if (transform_.rotate_.z > IDLE_ANGLE_Z + POURING_ANGLE_Z)
			{
				// 正規化された注ぎ中の最大角度まで倒れる時間
				float normalizedTime = std::clamp(pouringTime_ / 50.f, 0.0f, 1.f);

				// 注ぎ時の最大角度にかける係数
				float c = sinf((normalizedTime * XM_PI) / 2);

#ifdef _DEBUG
				ImGui::Begin("係数");
				ImGui::InputFloat("係数:", &c);
				ImGui::End();
#endif

				transform_.rotate_.z = IDLE_ANGLE_Z + (POURING_ANGLE_Z * c);

			}
			else if (transform_.rotate_.z < (POURING_ANGLE_Z + IDLE_ANGLE_Z) + bias)
			{
				pWFO_->Visible();
				pWFO_->WFVisible(true);
				pBeermug_->Fill();
			}

		}
		else
		{
			pWFO_->WFVisible(false);
			// 注ぎコマンドが入力されていない場合、待機時の角度に戻る
			if (transform_.rotate_.z < IDLE_ANGLE_Z)
			{
				pouringTime_ = 0;

				static float bias = 0.5f;
				transform_.rotate_.z = std::lerp(transform_.rotate_.z,
												 IDLE_ANGLE_Z + bias,
												 0.1f);
			}

			if (transform_.rotate_.z > -80.f)
			{
				pWFO_->Invisible();
			}
		}
	}

	isPouring_ = false;

#ifdef _DEBUG
	std::string panelName{ "ObjectTS" + objectName_ };

	ImGui::Begin(panelName.c_str());
	ImGui::InputFloat("pos.x", &transform_.position_.x);
	ImGui::InputFloat("pos.y", &transform_.position_.y);
	ImGui::InputFloat("pos.z", &transform_.position_.z);

	ImGui::InputFloat("rot.x", &transform_.rotate_.x);
	ImGui::InputFloat("rot.y", &transform_.rotate_.y);
	ImGui::InputFloat("rot.z", &transform_.rotate_.z);

	ImGui::InputFloat("scl.x", &transform_.scale_.x);
	ImGui::InputFloat("scl.y", &transform_.scale_.y);
	ImGui::InputFloat("scl.z", &transform_.scale_.z);
	ImGui::End();
#endif
}

void Bin::Draw()
{
	Model::SetTransform(hModel_, transform_);
	Model::Draw(hModel_);
}

void Bin::Release()
{
	binCount_ = 0;
}

void Bin::Pouring()
{
	if (not(isConfirmed_))
	{
		isPouring_ = true;
	}
}

void Bin::Conform()
{
	isConfirmed_ = true;
	transform_.rotate_.z = 0;
	transform_.position_.x += -0.25f;

	if (pWFO_)
	{
		pWFO_->Invisible();
	}
}
