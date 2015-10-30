#pragma once

#include "Utility/UtilityInterface.h"
#include "Utility/Factory/Archetype.h"
#include "ComponentFactoryBase.h"
#include "Core/CoreInterface.h"
#include "Physics/PhysicsInterface.h"

namespace WickedSick
{
  
  template <typename T>
  class ComponentFactory : public ComponentFactoryBase
  {
  public:
    ComponentFactory()
    {

    }
    Archetype<T>& GetArchetype(const std::string& name)
    {
      auto& object = archetype_map_.find(name);
      return (*object).val;
    }

    //hardcoded
    void AddArchetype(const std::string& name, Archetype<T>&& archetype)
    {
      archetype_map_.insert(name, archetype);
    }

    void LoadArchetypes(const std::string& dir)
    {
    }

    Component* Make(const std::string& archetypeName, GameObject* owner)
    {
      auto& archetype = archetype_map_.find(archetypeName);
      Component* toReturn = nullptr;
      if(archetype != archetype_map_.end())
      {
        toReturn = manager_.New((*archetype).val.GetBase());
        toReturn->SetOwner(owner);
      }
      
      return toReturn;
    }

    Component* Make(Archetype<T>& archetype, GameObject* owner)
    {
      archetype.GetBase().SetOwner(owner);
      return manager_.New(archetype.GetBase());
    }

    Component* MakeBlank()
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
    HashMap<std::string, Archetype<T> > archetype_map_;
    MemoryManager<T> manager_;
  };
}
