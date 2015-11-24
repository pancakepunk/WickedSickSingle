#include "Precompiled.h"
#include "TestingPrecompiled.h"
#include "HashMapTests.h"

namespace WickedSick
{
  void HashMap_print(const HashMap<std::string, int>& map)
  {
    for (auto it : map)
    { 
      ConsolePrint("[" + it.key + "]" + " = " + std::to_string(it.val));
      ConsolePrint("\n");
    }
    ConsolePrint("\n");
  }

  bool HashMap_insert()
  {
    HashMap<std::string, int> map;
    for (int i = 0; i < 20; ++i)
    {
      map.insert("Hello" + std::to_string(i), i);
      HashMap_print(map);
    }
    HashMap_print(map);
    return true;
  }

  bool HashMap_insert_stress()
  {
    HashMap<std::string, int> map;
    for (int i = 0; i < 2000; ++i)
    {
      map.insert("Hello" + std::to_string(i), i);
    }
    return (map.size() == 2000);
  }

  bool HashMap_erase()
  {
    HashMap<std::string, int> map;
    for (int i = 0; i < 20; ++i)
    {
      map.insert("Hello" + std::to_string(i), i);
    }

    for (int i = 0; i < 10; ++i)
    {
      map.erase("Hello" + std::to_string(i));
    }
    HashMap_print(map);

    return map.empty();
  }

  bool HashMap_erase2()
  {
    HashMap<std::string, int> map2;
    for (int i = 0; i < 20; ++i)
    {
      map2.insert("Hello" + std::to_string(i), i);
    }

    for (int i = 0; i < 10; ++i)
    {
      map2.erase("Hello" + std::to_string(i));
    }

    for (int i = 0; i < 20; ++i)
    {
      map2.insert("Hello" + std::to_string(i + 20), i + 20);
    }

    for (int i = 0; i < 20; ++i)
    {
      map2.insert("Hello" + std::to_string(i), i);
    }

    for (int i = 0; i < 20; ++i)
    {
      map2.erase("Hello" + std::to_string(i));
    }
    HashMap_print(map2);
    return true;
  }

  bool HashMap_erase_stress()
  {
    HashMap<std::string, int> map;
    for (int i = 0; i < 2000; ++i)
    {
      map.insert("Hello" + std::to_string(i), i);
    }
    for (int i = 0; i < 2000; ++i)
    {
      map.erase("Hello" + std::to_string(i));
    }
    return (!map.size());
  }

  bool HashMap_find()
  {
    HashMap<std::string, int> map;
    for (int i = 0; i < 20; ++i)
    {
      map.insert("Hello" + std::to_string(i), i);
    }

    auto it = map.find("Hello0");
    (*it).val = 420;

    auto sit = map.find("Hello5");
    (*sit).val = 69;
    
    auto tit = map.find("Hello0");
    ConsolePrint((*tit).key + std::to_string((*tit).val));

    auto fit = map.find("Hello5");
    ConsolePrint((*fit).key + std::to_string((*fit).val));
    
    return true;
  }

}