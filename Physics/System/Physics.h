#pragma once

#include "Core/CoreInterface.h"

namespace WickedSick
{
  class PhysicsScene;
  struct PhysicsMaterial;
  class PhysicsMesh;
  class PhysicsComponent;
  class GameObject;
  class Event;
  class Physics : public System
  {
    public:
    Physics();
    ~Physics();

    void Initialize() override;
    bool Load() override;
    bool Reload() override;
    void Update(float dt) override;
    void ReceiveMessage(Event* letter) override;

    PhysicsMaterial* GetMaterial(const std::string& name);
    PhysicsMesh* GetMesh(const std::string& name);
    PhysicsScene * GetScene();
      

    void AddComponent(PhysicsComponent* comp);
    std::set<PhysicsComponent*>& GetComponents();

      static Vector3 Gravity;

    private:
      
      //everything is still valid after this remove function
      template <typename T>
      static void remove(std::vector<T>& vec, int index, int& last)
      {
        std::swap(vec[index], vec[last]);
        --last;
      }
      
      std::set<PhysicsComponent*> comps_;
      HashMap<std::string, PhysicsMaterial*>   materials_;
      HashMap<std::string, PhysicsMesh*>       meshes_;
      PhysicsScene*                   scene_;
  };
}
