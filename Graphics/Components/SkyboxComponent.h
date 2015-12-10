#pragma once

#include "Core/CoreInterface.h"

namespace WickedSick
{

  class SkyboxComponent : public Component
  {
    public:
      SkyboxComponent();
      ~SkyboxComponent();
      void Initialize() override;
      void Update(float dt) override;
      void Clone(Component* source) override;

    private:
  };

}
