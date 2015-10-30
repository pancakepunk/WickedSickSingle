#pragma once


#include "Resolution/Contact.h"

namespace WickedSick
{
  class Collider;
  class Physics;
  class RigidBody;
  class BroadPhase;
  class PhysicsComponent;
  class Octree;
  class PhysicsScene
  {
    public:
    PhysicsScene();
    ~PhysicsScene();

    void Initialize();

    void Update(float dt);

    void Integrate(float dt);
    void CheckCollisions();
    void ResolveCollisions(float dt);
    
    void UnloadScene();
    void LoadScene();

    void SetLoaded(bool loaded);
    void AddPhysicsObject(PhysicsComponent* comp);

    void Refresh();

    void AddCollider(Collider* coll);
      
    std::vector<RigidBody*>& GetRigidBodies();

    BroadPhase* GetDynamicBroadphase();
    BroadPhase* GetStaticBroadphase();
    static bool GetIteration();
      
    private:
      std::vector<RigidBody*> rigid_bodies_;
      std::vector<Collider*> colliders_;

      BroadPhase* dynamic_broadphase_;
      BroadPhase* static_broadphase_;

      static bool resolution_iteration_;
      bool scene_loaded_;
      void find_basis(Contact& contact);
  };
}
