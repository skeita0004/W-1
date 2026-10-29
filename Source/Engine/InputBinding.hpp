#pragma once

#include "Input.hpp"

struct Pad
{
    unsigned int padID;
    unsigned int padButtonCode;
};

// mapで値の型として持たせてください。
struct InputBinding
{
    unsigned int keyCode;
    unsigned int mouseButtonCode;
    Pad  padInput;
};

static bool IsHold(const InputBinding& _binding)
{
    auto pad = _binding.padInput;
    return (Input::IsKey(_binding.keyCode) or
            Input::IsMouseButton(_binding.mouseButtonCode) or
            Input::IsPadButton(pad.padButtonCode, pad.padID));
}

static bool IsDown(const InputBinding& _binding)
{
    auto pad = _binding.padInput;
    return (Input::IsKeyDown(_binding.keyCode) or
            Input::IsMouseButtonDown(_binding.mouseButtonCode) or
            Input::IsPadButtonDown(pad.padButtonCode, pad.padID));
}

static bool IsUp(const InputBinding& _binding)
{
    auto pad = _binding.padInput;
    return (Input::IsKeyUp(_binding.keyCode) or
            Input::IsMouseButtonUp(_binding.mouseButtonCode) or
            Input::IsPadButtonUp(pad.padButtonCode, pad.padID));
}