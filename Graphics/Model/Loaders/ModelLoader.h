#pragma once
#include "GraphicsPrecompiled.h"

namespace WickedSick
{
  class Model;
  class ModelLoader
  {
  public:
    ModelLoader();
    virtual ~ModelLoader();
    virtual Model* Load(const std::string& source) = 0;
  };
}
