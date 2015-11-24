#include "Precompiled.h"

#include "PhysicsPrecompiled.h"
#include "Physics.h"
//engine
//#include "Engine.h"

//factory
//#include "Factory.h"
//#include "GameObject.h"
#include "Architecture/Collider.h"
#include "Physics.h"
#include "Components/PhysicsComponent.h"

//#include "Transform.h"
//events
//#include "Letter.h"
//#include "Letter_Type.h"
#include "Architecture/PhysicsScene.h"
#include "Architecture/PhysicsMaterial.h"
#include "Architecture/PhysicsMesh.h"
namespace WickedSick
{
  Vector3 Physics::Gravity;

  Physics::Physics() : System(ST_Physics)
  {
    Gravity = Vector3(0.0f, -9.8f, 0.0f);
  }

  Physics::~Physics()
  {
    for(auto it : meshes_)
    {
      delete it.val;
    }
    meshes_.clear();
    for(auto it : materials_)
    {
      delete it.val;
    }
    materials_.clear();
    if(scene_)
    {
      delete scene_;
    }
  }

  bool Physics::Load()
  {
    scene_ = new PhysicsScene();

    return true;
  }

  bool Physics::Reload()
  {
    return false;
  }

  void Physics::Initialize()
  {
    scene_->Initialize();
  }

  void Physics::Update(float dt)
  {
    scene_->Update(dt);

  }

  void Physics::ReceiveMessage(Event*)
  {
  }


  PhysicsMaterial* Physics::GetMaterial(const std::string & name) 
  { 
    auto it = materials_.find(name);
    if(it != materials_.end())
    {
      return (*it).val;
    }
    return nullptr;
  }

  PhysicsMesh* Physics::GetMesh(const std::string & name) 
  { 
    auto it = meshes_.find(name);
    if (it != meshes_.end())
    {
      return (*it).val;
    }
    return nullptr;
  }

  PhysicsScene* WickedSick::Physics::GetScene()
  {
    return scene_;
  }

  std::set<PhysicsComponent*>& Physics::GetComponents()
  {
    return comps_;
  }

  void Physics::AddComponent(PhysicsComponent * comp)
  {
    comps_.insert(comp);
  }
}



