#pragma once

#include "Math/MathInterface.h"


#include <vector>

namespace WickedSick
{
  class MatrixStack
  {
  public:
    MatrixStack();
    Matrix4 Top();
    void Push(const Matrix4& toPush);
    Matrix4 Pop(int num = 1);
  private:
    int current_index_;
    std::vector<Matrix4> mat_stack_;
  };
}
