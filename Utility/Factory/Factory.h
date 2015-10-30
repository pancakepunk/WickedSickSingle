#pragma once

#include "Utility/UtilityInterface.h"
#include "Archetype.h"
//#include "FactoryBase.h"
namespace WickedSick
{
  template <typename T>
  class Factory
  {
  public:
    Factory()
    {

    }

    Archetype<T>& GetArchetype(const std::string& name)
    {
      auto& object = archetype_map_.find(name);
      return object->second;
    }

    std::unordered_map<std::string, Archetype<T> >& GetArchetypes()
    {
      return archetype_map_;
    }

    //hardcoded
    void AddArchetype(std::string&& name, Archetype<T>&& archetype)
    {
      archetype_map_.insert(std::make_pair(std::forward<std::string>(name), std::forward<Archetype<T>>(archetype)));
    }

    void LoadArchetypes(const std::string& dir)
    {
    }

    T* Make(const std::string& archetypeName)
    {
      auto& archetype = archetype_map_.find(archetypeName);
      if(archetype != archetype_map_.end())
      {
        return manager_.New(archetype->second.GetBase());
      }
      return nullptr;
    }

    T* Make(const Archetype<T>& archetype)
    {

    }

    T* MakeBlank()
    {
      return manager_.New();
    }

    template<typename ...Types>
    T* Create(const Types&... args)
    {
      return manager_.New(args...);
    }

    void Destroy(T* toDestroy)
    {
      manager_.Delete(toDestroy);
    }

  private:
    std::unordered_map<std::string, Archetype<T> > archetype_map_;
    MemoryManager<T> manager_;
  };
}
