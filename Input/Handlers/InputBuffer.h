#pragma once
#include "InputPrecompiled.h"

#include "Math/MathInterface.h"
#include "InputEnums.h"

namespace WickedSick
{
  struct Key
  {
    Key();
    int key;
    float sinceReleased;
    bool inputType[InputType::Count];
  };

  struct InputBuffer
  {
    InputBuffer();
    bool Get(int index);
    void Set(int index, bool value);
    bool down[MaxKeys];
    bool current_modifiers_[ModifierType::Count];
    Vector2i mousePos;
    int wheelChange;
  };
}

