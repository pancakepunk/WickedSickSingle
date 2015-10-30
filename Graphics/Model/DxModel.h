#pragma once

#include "Model/Model.h"

namespace WickedSick
{
  class DxModel : public Model
  {
  public:
    DxModel();
    ~DxModel();
    void Initialize() override;
    void Render() override;
    void ReInitBuffers() override;
    
  private:
  };
}
