#pragma once
#include "Utility/HashMap.h"
#include <string>
#include <set>

namespace WickedSick
{
  class Component;
  class ComponentFactoryBase;
  class GameObject;


  class ComponentManager
  {
  public:
    ComponentManager();
    ~ComponentManager();
    void Initialize();
    void Update(float dt);
    Component* CreateComponent( const std::string & name,
                                GameObject* owner);
    Component* CreateComponent( const std::string& name, 
                                const std::string& archetype,
                                GameObject* owner);

    void DestroyComponent(Component* comp);

    ComponentFactoryBase* GetFactory(const std::string& name);

  private:

    HashMap<std::string, ComponentFactoryBase*> factory_map_;

    std::vector<std::set<Component*>> component_list_;



  };
}
