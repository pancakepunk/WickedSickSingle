#pragma once

#include "Component.h"
#include "Transform.h"
#include "Utility/UtilityInterface.h"

namespace WickedSick
{
  class GameObject
  {
  public:
    GameObject(int goID = invalidID);
    GameObject(GameObject && rhs);
    GameObject(const GameObject& rhs);

    void Initialize();
    void Clone(GameObject* newObj);
    int GetID() const;
    std::string GetArchetypeName() const;
    std::string GetName() const;

    void SetID(int id);
    void SetArchetypeName(const std::string& name);
    void SetName(const std::string& name);

    void Activate();
    void Deactivate();
    bool GetActive();
    void AddComponent(Component* newComp);
    bool Valid() const;
    Component* GetComponent(ComponentType type);
    std::vector<Component*>& GetComponents();
    static const int invalidID = -1;
  private:
    static Factory<Transform> transforms_;
  //asdfsf<asdfds/>
    std::string archetype_name_;
    std::string name_;
    std::vector<Component*> components_;
    int go_id_;
    bool active_;
  };
}
