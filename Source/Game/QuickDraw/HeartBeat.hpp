#pragma once

class HeartBeat
{
public:
    void Start();
    void Stop();
    void Update(float deltaTime);

    bool IsActive() const
    {
        return active_;
    }

private:
    void SetVibration(int power);
    void Pulse();

    bool active_ = false;

    float elapsed_ = 0.0f;
    float beatTimer_ = 0.0f;
    float pulseTimer_ = 0.0f;

    float beatInterval_ = 1.2f;
    float pulseDuration_ = 0.15f;

    int power_ = 15000;
    int maxPower_ = 60000;

    bool pulsing_ = false;
};