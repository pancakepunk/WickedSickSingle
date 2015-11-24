#pragma once
#include "Math/MathInterface.h"

namespace WickedSick
{

  struct Vertex
  {
    bool operator==(const Vertex& rhs);
    Vector3 position;
    Vector3 normal;
    Vector3 tangent;
    Vector3 bitangent;
    Vector2 tex;

    std::string to_string();
    void Print();
  };
}
