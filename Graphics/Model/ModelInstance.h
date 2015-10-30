#pragma once

#include "Math/MathInterface.h"

namespace WickedSick
{
  class Model;
  struct ModelInstance
  {
    ModelInstance();
    void Destroy();
    Model* parent;
  };
}