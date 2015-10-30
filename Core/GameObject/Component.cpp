#include "CorePrecompiled.h"
#include "Component.h"
#include "Debugging/DebugInterface.h"
#include "GameObject.h"

namespace WickedSick
{
  Component::Component( ComponentType type, 
                                    GameObject* owner)
                                    : type_(type),
                                      owner_(owner),
                                      active_(false)
  {
    //if (!owner_)
    //{
    //  ConsolePrint("Did you explicitly construct the base Component?");
    //}
  }
  
  Component::~Component()
  {

  }

  std::string Component::GetName()
  {
    
    char* names[] = 
    {
    #define RegisterComponentType(x) #x,
    #include "ComponentTypes.h"
    #undef RegisterComponentType
    };
    return names[type_];
  }

  ComponentType Component::GetType()
  {
    return type_;
  }

  bool Component::GetActive()
  {
    return active_;
  }

  void Component::Activate()
  {
    active_ = true;
  }

  void Component::Deactivate()
  {
    active_ = false;
  }

  void Component::SetDebug(bool newDebug)
  {
    render_debug_ = newDebug;
  }

  Component * Component::GetSibling(ComponentType type)
  {
    return owner_->GetComponent(type);
  }

  GameObject * Component::GetOwner()
  {
    return owner_;
  }

  void Component::SetOwner(GameObject* newOwner)
  {
    owner_ = newOwner;
  }

  void Component::render_debug()
  {

  }

}
