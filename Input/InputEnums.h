#pragma once
namespace WickedSick
{

  namespace InputType
  {
    enum Enum
    {
      Toggled,
      Pressed,
      DoubleToggled,
      DoublePressed,
      Released,
      Scrolled,
      Count

    };
  }

  namespace ModifierType
  {
    enum Enum
    {
      Control,
      Shift,
      Alt,
      Count
    };
  }
}
