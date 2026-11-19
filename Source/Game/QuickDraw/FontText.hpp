#pragma once

#include <Windows.h>

struct ImFont;

class FontText
{
public:
	FontText() = default;

	HRESULT Initialize(const char* fileName, float fontSize = 24.0f);

	void Draw(float x, float y, const char* text, unsigned int color = 0xFFFFFFFF) const;

	void Release();

private:
	ImFont* font_ = nullptr;
	float fontSize_ = 24.0f;
};
