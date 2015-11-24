#include "Precompiled.h"
#include "CorePrecompiled.h"
#include "FrameController.h"

namespace WickedSick
{
  double FrameController::elapsed_ = 0.0f;
  FrameController::FrameController() :  max_accumulator_(2.0f),
                                        frame_time_(0.0),
                                        time_step_(1.0f/60.0f),
                                        accumulator_(0.0),
                                        fps_(0)
  {
    last_time_ = { {0} };
    clocks_per_cycle_ = { {0} };
    QueryPerformanceFrequency(&clocks_per_cycle_);
    QueryPerformanceCounter(&last_time_);
  }

  FrameController::~FrameController()
  {

  }

  bool FrameController::Step()
  {
    if (accumulator_ >= time_step_)
    { 
      accumulator_ -= time_step_;
      ++frame_count_;
      return true;
    }
    return false;
  }

  void FrameController::Update()
  {
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    LARGE_INTEGER timePassed;
    timePassed.QuadPart = currentTime.QuadPart - last_time_.QuadPart;

    frame_time_ = (double)timePassed.QuadPart / (double)clocks_per_cycle_.QuadPart;
    last_elapsed_ = elapsed_;
    accumulator_ += frame_time_;
    elapsed_ += frame_time_;
    if ((int)last_elapsed_ != (int)elapsed_)
    {
      fps_ = frame_count_;
      frame_count_ = 0;
    }
    accumulator_ = std::min(accumulator_, max_accumulator_);

    last_time_ = currentTime;
  }

  void FrameController::SetTimeStep(double time_step)
  {
    time_step_ = time_step;
  }

  float FrameController::GetTimeStep()
  {
    return (float)time_step_;
  }

  float FrameController::GetFrameTime()
  {
    return (float)frame_time_;
  }

  double FrameController::CurrentTime()
  {
    return elapsed_;
  }
}
