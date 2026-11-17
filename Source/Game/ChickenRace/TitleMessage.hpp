#pragma once

#include "../Engine/GameObject.h"

class Text;

class TitleMessage : public GameObject
{
public:
	TitleMessage(GameObject* _pParent);

	void Initialize() override;
	void Update()     override;
	void Draw()       override;
	void Release()    override;

	bool& ChangeScene()
	{
		return changeScene_;
	}

private:
	Text* pText_;

	bool changeScene_;

	inline static const int BLINK_OUT_COUNT{120};
	inline static const int LIGHT_UP_COUNT{60};
};