#include "Time.h"
#include <Windows.h>

namespace
{
	LARGE_INTEGER freq;
	LARGE_INTEGER current;
	LARGE_INTEGER last;

	float deltaTime = 0.0f;

	// スムージング用バッファ
	const int BUF_SIZE = 30;
	float timeBuf[BUF_SIZE];
	int readP = 0;
	int writeP = 0;
}

void Time::Initialize()
{
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&current);
	last = current;

	readP = 0;
	writeP = 0;
}

void Time::Refresh()
{
	last = current;
	QueryPerformanceCounter(&current);

	float dt =
		static_cast<float>(current.QuadPart - last.QuadPart)
		/ static_cast<float>(freq.QuadPart);

	// バッファに入れる
	timeBuf[writeP] = dt;
	writeP = (writeP + 1) % BUF_SIZE;

	if (writeP == readP)
	{
		readP = (readP + 1) % BUF_SIZE;
	}

	// 平均計算（スムージング）
	float sum = 0.0f;
	int num = 0;

	for (int i = readP; i != writeP; i = (i + 1) % BUF_SIZE)
	{
		sum += timeBuf[i];
		num++;
	}

	if (num > BUF_SIZE / 2)
		deltaTime = sum / num;
	else
		deltaTime = dt;

	// フリーズ防止
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;
}

float Time::DeltaTime()
{
    return deltaTime;
}

void Time::Release()
{
}
