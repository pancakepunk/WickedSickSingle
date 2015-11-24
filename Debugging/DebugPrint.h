#pragma once

#include "DebugInterface.h"


namespace WickedSick
{
  enum ConsoleColor
  {
    ConsoleDarkBlue = 1,
    ConsoleGreen,
    ConsoleGrey,
    ConsoleDarkRed,
    ConsoleDarkPurple,
    ConsoleDarkBrown,
    ConsoleLightGrey,
    ConsoleDarkGrey,
    ConsoleBlue,
    ConsoleLimeGreen,
    ConsoleTeal,
    ConsoleRed,
    ConsolePurple,
    ConsoleYellow,
    ConsoleWhite
  };

  void ConsolePrint(const std::string& contents,
                                 ConsoleColor color = ConsoleWhite);
}
