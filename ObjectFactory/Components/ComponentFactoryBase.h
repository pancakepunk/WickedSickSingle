#pragma once
#include <string>
namespace WickedSick
{
  class Component;
  class GameObject;
  class ComponentFactoryBase
  {
  public:
    ComponentFactoryBase();
    virtual ~ComponentFactoryBase();
    virtual Component* Make(const std::string& archetypeName, GameObject* owner) = 0;
    virtual Component* MakeBlank() = 0;
  private:
  };
}
