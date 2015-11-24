#include "Precompiled.h"
#include "CorePrecompiled.h"
#include "GameObject.h"

namespace WickedSick
{

  Factory<Transform> GameObject::transforms_;
  GameObject::GameObject( int goID)
                                      : go_id_(goID),
                                        active_(false)
  {
    components_.resize(CT_Count, nullptr);
    for (int i = 0; i < CT_Count; ++i)
    {
      components_[i] = nullptr;
    }
    AddComponent(transforms_.Create(this));
  }

  GameObject::GameObject( GameObject&& rhs) 
                                      : go_id_(std::move(rhs.go_id_)),
                                        name_(std::move(rhs.name_)),
                                        active_(std::move(rhs.active_)),
                                        archetype_name_(std::move(rhs.archetype_name_))
  {
    components_.resize(CT_Count, nullptr);
    for (int i = 0; i < CT_Count; ++i)
    {
      components_[i] = rhs.components_[i];
      if (components_[i])
      {
        components_[i]->SetOwner(this);
      }
      rhs.components_[i] = nullptr;
    }
  }

  GameObject::GameObject( const GameObject& rhs) 
                                      : name_(rhs.name_),
                                        archetype_name_(rhs.archetype_name_)
  {
    components_.resize(CT_Count, nullptr);
    for (int i = 0; i < CT_Count; ++i)
    {
      components_[i] = nullptr;
    }
  }


  bool GameObject::Valid() const
  {
    return go_id_ != GameObject::invalidID;
  }

  Component * GameObject::GetComponent(ComponentType type)
  {
    return components_[type];
  }

  std::vector<Component*>& GameObject::GetComponents()
  {
    return components_;
  }

  int GameObject::GetID() const
  {
    return go_id_;
  }

  std::string GameObject::GetArchetypeName() const
  {
    return archetype_name_;
  }

  std::string GameObject::GetName() const
  {
    return name_;
  }

  void GameObject::SetID(int id)
  {
    go_id_ = id;
  }

  void GameObject::SetArchetypeName(const std::string& name)
  {
    archetype_name_ = name;
  }

  void GameObject::SetName(const std::string& name)
  {
    name_ = name;
  }

  void GameObject::Activate()
  {
    active_ = true;
    for(auto& it : components_)
    {
      if(it)
      {
        it->Activate();
      }
    }
  }

  void GameObject::Deactivate()
  {
    active_ = false;
    for (auto& it : components_)
    {
      if(it)
      {
        it->Deactivate();
      }
    }
  }


  bool GameObject::GetActive()
  {
    return active_;
  }

  void GameObject::Initialize()
  {
    for (auto& it : components_)
    {
      if (it)
      {
        it->Initialize();
      }
    }
  }

  void GameObject::Clone(GameObject* newObj)
  {

  }

  void GameObject::AddComponent(Component* newComp)
  {
    if(newComp)
    {
      components_[newComp->GetType()] = newComp;
      newComp->SetOwner(this);
    }
  }

}
