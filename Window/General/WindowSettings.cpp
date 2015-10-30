#include "WindowPrecompiled.h"
#include "Window/General/WindowSettings.h"

#include "Window/General/WindowType.h"

namespace WickedSick
{
  //most basic settings are default so we don't break their machine.
  //if they can't handle this then they can't handle our game.
  WindowSettings::WindowSettings() :  resolution(Vector2i(800, 600)),
                                      bitColor(16),                   
                                      windowType(WT_Windowed),
                                      refreshRate(60)
  {

  }
}
