#pragma once
#include "Math/MathInterface.h"

namespace WickedSick
{

  struct Vertex
  {
    bool operator==(const Vertex& rhs);
    Vector3 position;
    Vector3 normal;
    Vector2 tex;

    std::string to_string();
    void Print();
  };
}
