#include "Precompiled.h"
#include "UtilityPrecompiled.h"



#include <iostream>

namespace WickedSick
{

  std::string TrimSpaces(std::string toTrim)
  {
    for (auto& it = toTrim.begin() + 1; it != toTrim.end(); ++it)
    {
      if (*it == ' ' && *(it - 1) == ' ')
      {
        it = toTrim.erase(it);
      }
    }
    return toTrim;
  }
}