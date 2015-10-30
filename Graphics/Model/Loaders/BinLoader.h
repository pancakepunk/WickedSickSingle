#pragma once

#include "ModelLoader.h"

namespace WickedSick
{
  class Model;
  class BinLoader : public ModelLoader
  {
  public:
    BinLoader();
    ~BinLoader();
    Model* Load(const std::string& source);
  private:
  };
}
