#include "Precompiled.h"
#include "DebugPrecompiled.h"

#include "DebugPrint.h"

#include "windows.h"


namespace WickedSick
{

  void ConsolePrint(const std::string& contents,
                                 ConsoleColor color)
  {
    HANDLE hcon = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hcon, color);
    std::cout << contents;
  }
}
