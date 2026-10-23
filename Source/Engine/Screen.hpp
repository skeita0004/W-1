#pragma once

namespace Screen
{
    const float WIDTH{16};
    const float HEIGHT{9};

    const int FPS{60};

    inline float GetAspectRatio()
    {
        return WIDTH / HEIGHT;
    }
}