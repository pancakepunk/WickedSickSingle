#pragma once

namespace WickedSick
{
  class GameObject;
  enum ComponentType
  {
    #define RegisterComponentType(x) CT_##x,
    #include "ComponentTypes.h"
    #undef RegisterComponentType
    CT_Count
  };
  class Component
  {
  public:

    

    Component(ComponentType type = CT_Count, 
              GameObject* owner = nullptr);
    virtual ~Component();

    virtual void Initialize() = 0;
    virtual void Update(float dt) = 0;
    

    virtual std::string GetName() final;
    virtual ComponentType GetType() final;
    virtual bool GetActive() final;
    virtual void Activate() final;
    virtual void Deactivate() final;
    virtual void SetDebug(bool newDebug) final;

    virtual void Clone(Component* source) = 0;

    virtual Component* GetSibling(ComponentType type) final;

    virtual GameObject* GetOwner() final;
    virtual void SetOwner(GameObject* newOwner) final;

  private:

    virtual void render_debug();
    bool        active_;
    bool        render_debug_;
    ComponentType        type_;
    GameObject* owner_;

  };
  
}
