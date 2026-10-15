#pragma once

#include <string>

namespace Debug
{
	void Log(int value, bool isLineFeed = false);
	void Log(float value, bool isLineFeed = false);
	void Log(std::string value, bool isLineFeed = false);
};

