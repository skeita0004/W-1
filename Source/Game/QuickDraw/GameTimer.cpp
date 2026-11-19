#include "GameTimer.h"

void GameTimer::Reset()
{
    targetSeconds_ = 0;
    elapsedSeconds_ = 0.0;
    waiting_ = false;
    running_ = false;
    stopped_ = false;
    startTime_ = Clock::time_point{};
}

void GameTimer::Start(int targetSeconds)
{
    Reset();
    targetSeconds_ = targetSeconds;
    startTime_ = Clock::now();
    waiting_ = true;
}

void GameTimer::Update()
{
    if (!waiting_)
        return;

    const auto now = Clock::now();
    const std::chrono::duration<double> elapsed = now - startTime_;

    if (elapsed.count() >= WAIT_SECONDS)
    {
        startTime_ = now;
        waiting_ = false;
        running_ = true;
    }
}

double GameTimer::Stop()
{
    if (running_)
    {
        elapsedSeconds_ =
            std::chrono::duration<double>(Clock::now() - startTime_).count();
        running_ = false;
        stopped_ = true;
    }
    return elapsedSeconds_;
}

bool GameTimer::IsWaiting() const { return waiting_; }
bool GameTimer::IsRunning() const { return running_; }
bool GameTimer::IsStopped() const { return stopped_; }
int GameTimer::GetTargetSeconds() const { return targetSeconds_; }

double GameTimer::GetElapsedSeconds() const
{
    if (running_)
        return std::chrono::duration<double>(Clock::now() - startTime_).count();

    return elapsedSeconds_;
}
