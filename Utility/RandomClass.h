#pragma once
#include <random>
namespace WickedSick
{
  class Random
  {
    public:
      Random();
      float Gen();
    private:
      std::random_device device_;//?
      std::mt19937 generator_;
      std::uniform_real_distribution<float> normal_distribution_;
  };
}
