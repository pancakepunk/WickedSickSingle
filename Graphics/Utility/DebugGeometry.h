#pragma once
#include "Math/MathInterface.h"
#include "Model/Vertex.h"
namespace WickedSick
{
  struct DebugLine
  {
    DebugLine(const Vertex& start,
              const Vertex& end);
    Vertex Start;
    Vertex End;
  };
}