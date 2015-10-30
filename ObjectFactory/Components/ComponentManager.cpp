#include "ObjectFactoryPrecompiled.h"
#include "ComponentManager.h"
#include "Core/CoreInterface.h"
#include "Components/ComponentFactory.h"
#include "Graphics/GraphicsInterface.h"
#include "Physics/PhysicsInterface.h"
//#include ""

namespace WickedSick
{
  ComponentManager::ComponentManager()
  {
    component_list_.resize(CT_Count);
#define RegisterComponentType(x) factory_map_.insert(std::string(#x), (ComponentFactoryBase*)new ComponentFactory<x>());
#include "Core/GameObject/ComponentTypes.h"
#undef RegisterComponentType

  }

  ComponentManager::~ComponentManager()
  {
  }

  void ComponentManager::Initialize()
  {
    
  }

  void ComponentManager::Update(float dt)
  {
    for(auto& it : component_list_)
    {
      for(auto& cit : it)
      {
        if(cit->GetActive())
        {
          cit->Update(dt);
        }
      }
    }
  }

  Component* ComponentManager::CreateComponent(const std::string & name,
                                               GameObject* owner)
  {
    Component* newComp = nullptr;
    auto& factoryIt = factory_map_.find(name);
    if (factoryIt != factory_map_.end())
    {
      newComp = (*factoryIt).val->MakeBlank();
      newComp->SetOwner(owner);
      component_list_[newComp->GetType()].insert(newComp);
    }



    return newComp;
  }

  Component* ComponentManager::CreateComponent( const std::string & name, 
                                                const std::string& archetype, 
                                                GameObject* owner)
  {
    Component* newComp = nullptr;
    auto& factoryIt = factory_map_.find(name);
    if (factoryIt != factory_map_.end())
    {
      newComp = (*factoryIt).val->Make(archetype, owner);
      if(newComp)
      {
        component_list_[newComp->GetType()].insert(newComp);
      }
    }
    return newComp;
  }

  void ComponentManager::DestroyComponent(Component * comp)
  {
    auto& compList = component_list_[comp->GetType()];
    auto it = compList.find(comp);
    if(it != compList.end())
    {
      compList.erase(comp);
    }

  }

  ComponentFactoryBase* ComponentManager::GetFactory(const std::string& name)
  {
    auto& factoryIt = factory_map_.find(name);
    return (factoryIt != factory_map_.end()) ? (*factoryIt).val : nullptr;
  }

}
