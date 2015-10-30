#pragma once



namespace WickedSick
{
  class FrameController
  {
  public:
    FrameController();
    ~FrameController();
    bool Step();
    void Update();
    void SetTimeStep(double time_step);
    float GetTimeStep();
    float GetFrameTime();
    static double CurrentTime();
  private:
    LARGE_INTEGER last_time_;
    LARGE_INTEGER clocks_per_cycle_;
    
    double        max_accumulator_;
    double        frame_time_;
    double        time_step_;
    double        accumulator_;
    double        last_elapsed_;
    static double        elapsed_;
    short         frame_count_;
    short         fps_;
  };
}
