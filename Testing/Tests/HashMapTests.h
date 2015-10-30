#pragma once

#include "Utility/UtilityInterface.h"

#include "Debugging/DebugInterface.h"

namespace WickedSick
{

  void HashMap_print(const HashMap<std::string, int>& map);

  bool HashMap_insert();
  bool HashMap_insert_stress();

  bool HashMap_erase();
  bool HashMap_erase2();
  bool HashMap_erase_stress();
  
  bool HashMap_find();
}
