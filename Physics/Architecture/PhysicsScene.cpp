#include "Precompiled.h"
#include "PhysicsPrecompiled.h"
#include "PhysicsScene.h"


#include "RigidBody.h"
#include "Resolution/ContactManifold.h"
#include "Resolution/ContactEdge.h"
#include "Collider.h"
#include "PhysicsMesh.h"
#include "PhysicsMaterial.h"

#include "System/Physics.h"

#include "Resolution/ConstraintList.h"
#include "Collider.h"
#include "Components/PhysicsComponent.h"
#include "PhysicsScene.h"

namespace WickedSick
{
  bool PhysicsScene::resolution_iteration_ = false;

  PhysicsScene::PhysicsScene() : dynamic_broadphase_(nullptr),
                                 static_broadphase_(nullptr),
                                 scene_loaded_(false)
  {
  }

  PhysicsScene::~PhysicsScene()
  {
  }

  void PhysicsScene::Initialize()
  {

  }

  void PhysicsScene::Update(float dt)
  {
    if(scene_loaded_)
    {
      Integrate(dt);
      CheckCollisions();
      ResolveCollisions(dt);
    }
  }

  void PhysicsScene::Integrate(float dt)
  {
    for(auto& it : rigid_bodies_)
    {
      if(!it->IsStatic())
      {
        it->Integrate(dt);
      }
    }
  }

  // super basic pretty awful looking n-squared aabb check
  void PhysicsScene::CheckCollisions()
  {
    Vector3 Position1;
    Vector3 Position2;
    RigidBody* first;
    RigidBody* second;
    if(rigid_bodies_.empty())
    {
      return;
    }
    for(auto bodyit = rigid_bodies_.begin(); bodyit != rigid_bodies_.end() - 1; ++bodyit)
    {
      first = (*bodyit);
      if(!first->GetBase()->GetActive())
      {
        continue;
      }
      for(auto bodysit = bodyit + 1; bodysit != rigid_bodies_.end(); ++bodysit)
      {
        second = (*bodysit);
        if(!second->GetBase()->GetActive())
        {
          continue;
        }
        if(!first->IsStatic() || !second->IsStatic())
        {
          for(auto& colliderit : first->GetColliders())
          {
            Position1 = first->GetState().Position + first->GetState().CenterOfMass + colliderit->GetPosition();
            for(auto& collidersit : second->GetColliders())
            {
              Position2 = second->GetState().Position + second->GetState().CenterOfMass + collidersit->GetPosition();
              if(colliderit->GetCollidingIds().find(collidersit->GetId()) != colliderit->GetCollidingIds().end())
              {
                float v0Dotp0p1;
                float v1Dotp1p0;
                Vector3 line = second->GetState().Position - first->GetState().Position;
                v0Dotp0p1 = first->GetState().Velocity.Dot(line);
                v1Dotp1p0 = second->GetState().Velocity.Dot(-line);
                if(!first->GetState().Velocity.LengthSq() || !second->GetState().Velocity.LengthSq())
                {
                  //we just set it so it'll check
                  v0Dotp0p1 = 1.0f;
                }
                if((v0Dotp0p1 > 0.0f) || (v1Dotp1p0 > 0.0f))
                {
                  switch(colliderit->GetMesh()->GetType())
                  {
                    case PhysicsMeshType::AaBb:
                    {
                      switch(collidersit->GetMesh()->GetType())
                      {
                        case PhysicsMeshType::AaBb:
                        {
                          Vector3 difference = Position1 - Position2;
                          difference.x = fabs(difference.x);
                          difference.y = fabs(difference.y);
                          difference.z = fabs(difference.z);
                          difference -= (colliderit->GetMesh()->GetBoundingBox().max_ + collidersit->GetMesh()->GetBoundingBox().max_);
                          float pd = std::max(std::max(difference.x, difference.y), difference.z);
                          if(pd < 0.0f)
                          {
                            Contact toadd;
                            Vector3 position;
                            Vector3 normal;
                            //if difference is positive, then contact points are points - difference
                            //so

                            if(pd == difference.x)
                            {
                              normal.x = 1.0f;
                            }
                            else if(pd == difference.y)
                            {
                              normal.y = 1.0f;
                            }
                            else if(pd == difference.z)
                            {
                              normal.z = 1.0f;
                            }
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(!edge)//colliding
                            {
                              edge = colliderit->AddEdgeWith(collidersit);
                            }

                            Vector3 contact1( clamp(Position2.x, Position1.x + colliderit->GetMesh()->GetBoundingBox().min_.x, Position1.x + colliderit->GetMesh()->GetBoundingBox().max_.x),
                                              clamp(Position2.y, Position1.y + colliderit->GetMesh()->GetBoundingBox().min_.y, Position1.x + colliderit->GetMesh()->GetBoundingBox().max_.y),
                                              clamp(Position2.z, Position1.z + colliderit->GetMesh()->GetBoundingBox().min_.z, Position1.x + colliderit->GetMesh()->GetBoundingBox().max_.z));

                            Vector3 contact2( clamp(Position1.x, Position2.x + collidersit->GetMesh()->GetBoundingBox().min_.x, Position2.x + collidersit->GetMesh()->GetBoundingBox().max_.x),
                                              clamp(Position1.y, Position2.y + collidersit->GetMesh()->GetBoundingBox().min_.y, Position2.x + collidersit->GetMesh()->GetBoundingBox().max_.y),
                                              clamp(Position1.z, Position2.z + collidersit->GetMesh()->GetBoundingBox().min_.z, Position2.x + collidersit->GetMesh()->GetBoundingBox().max_.z));

                            toadd.SetWorldContactPoint1(contact1);
                            toadd.SetWorldContactPoint2(contact2);
                            toadd.SetNormal(normal);
                            toadd.SetPenetrationDepth(pd);
                            toadd.find_basis();
                            edge->GetManifold()->AddContact(&toadd);

                          }
                          else
                          {
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(edge)
                            {
                              colliderit->RemoveEdgeWith(collidersit);
                            }
                          }
                          break;
                        }
                        
                        // AABB to AABB case

                        case PhysicsMeshType::Sphere:
                        {
                          //for all points in a box, find the shortest distance from them to the sphere

                          float radius = collidersit->GetMesh()->GetRadius();
                          Vector3 collisionPoint = Position2;
                          collisionPoint.x = clamp( collisionPoint.x, 
                                                    Position2.x + colliderit->GetMesh()->GetBoundingBox().min_.x, 
                                                    Position2.x + colliderit->GetMesh()->GetBoundingBox().max_.x);

                          collisionPoint.y = clamp( collisionPoint.y, 
                                                    Position2.y + colliderit->GetMesh()->GetBoundingBox().min_.y, 
                                                    Position2.y + colliderit->GetMesh()->GetBoundingBox().max_.y);

                          collisionPoint.z = clamp( collisionPoint.z, 
                                                    Position2.z + colliderit->GetMesh()->GetBoundingBox().min_.z, 
                                                    Position2.z + colliderit->GetMesh()->GetBoundingBox().max_.z);
                          //we have to clamp the position of the sphere to the bounding box to get the closest point to the sphere on the box
                          Vector3 collisionNormal = collisionPoint - Position2;



                          if(collisionNormal.LengthSq() < (radius * radius))
                          {
                            Contact toadd;
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(!edge)//not colliding
                            {
                              edge = colliderit->AddEdgeWith(collidersit);
                            }

                            float normalLength = collisionNormal.Length();
                            toadd.SetPenetrationDepth(radius - normalLength);
                            collisionNormal.Normalize();
                            toadd.SetNormal(collisionNormal);
                            toadd.SetWorldContactPoint2((toadd.GetNormal() * radius) + Position2);
                            toadd.SetWorldContactPoint1(collisionPoint + Position1);
                            toadd.find_basis();
                            edge->GetManifold()->AddContact(&toadd);
                          }
                          else
                          {
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(edge)
                            {
                              colliderit->RemoveEdgeWith(collidersit);
                            }
                          }
                          break;
                        } // AABB to Sphere case
                      }
                      break;
                    }
                    case PhysicsMeshType::Sphere:
                    {
                      switch(collidersit->GetMesh()->GetType())
                      {
                        case PhysicsMeshType::AaBb:
                        {
                          //for all points in a box, find the shortest distance from them to the sphere
                          float radius = colliderit->GetMesh()->GetRadius();
                          Vector3 collisionPoint = Position1;
                          Contact toadd;
                          collisionPoint.x = clamp( collisionPoint.x, 
                                                    Position2.x + collidersit->GetMesh()->GetBoundingBox().min_.x, 
                                                    Position2.x + collidersit->GetMesh()->GetBoundingBox().max_.x);

                          collisionPoint.y = clamp( collisionPoint.y, 
                                                    Position2.y + collidersit->GetMesh()->GetBoundingBox().min_.y, 
                                                    Position2.y + collidersit->GetMesh()->GetBoundingBox().max_.y);

                          collisionPoint.z = clamp( collisionPoint.z, 
                                                    Position2.z + collidersit->GetMesh()->GetBoundingBox().min_.z, 
                                                    Position2.z + collidersit->GetMesh()->GetBoundingBox().max_.z);
                          //we have to clamp the position of the sphere to the bounding box to get the closest point to the sphere on the box

                          Vector3 collisionNormal = collisionPoint - Position1;
                          if(collisionNormal.LengthSq() < (radius * radius))
                          {
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(!edge)//not colliding
                            {
                              edge = colliderit->AddEdgeWith(collidersit);
                            }

                            float normalLength = collisionNormal.Length();
                            toadd.SetPenetrationDepth(radius - normalLength);
                            collisionNormal.Normalize();
                            toadd.SetNormal(collisionNormal);
                            toadd.SetWorldContactPoint1((toadd.GetNormal() * radius) + Position2);
                            toadd.SetWorldContactPoint2(collisionPoint + Position1);
                            toadd.find_basis();
                            edge->GetManifold()->AddContact(&toadd);
                          }
                          else
                          {
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(edge)
                            {
                              colliderit->RemoveEdgeWith(collidersit);
                            }
                          }
                          break;
                        } // Sphere to AABB
                        case PhysicsMeshType::Sphere:
                        {
                          float overlap = (Position2 - Position1).Length() - (colliderit->GetMesh()->GetRadius() + collidersit->GetMesh()->GetRadius());
                          if(overlap < 0.0f)
                          {
                            Vector3 difference = Position2 - Position1;
                            Contact toadd;
                            float radTotal = colliderit->GetMesh()->GetRadius() + collidersit->GetMesh()->GetRadius();

                            //we're colliding
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(!edge)//colliding
                            {
                              edge = colliderit->AddEdgeWith(collidersit);
                            }

 
                            toadd.SetNormal(difference.GetNormalized());
                            if(toadd.GetNormal() == Vector3())
                            {
                              toadd.SetNormal(Vector3(1.0f, 0.0f, 0.0f));
                            }
                            toadd.SetWorldContactPoint1(toadd.GetNormal() * colliderit->GetMesh()->GetRadius() + Position1);
                            toadd.SetWorldContactPoint2(-toadd.GetNormal() * collidersit->GetMesh()->GetRadius() + Position2);
                            toadd.SetPenetrationDepth(overlap);
                            toadd.find_basis();
                            edge->GetManifold()->AddContact(&toadd);
                          }
                          else
                          {
                            ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                            if(edge)
                            {
                              colliderit->RemoveEdgeWith(collidersit);
                            }
                          }
                          break;
                        } // Sphere to Sphere
                      }
                      break;
                    } 
                  }
                }
                else
                {
                  ContactEdge* edge = colliderit->GetEdgeWith(collidersit);
                  if(edge)
                  {
                    colliderit->RemoveEdgeWith(collidersit);
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  void PhysicsScene::ResolveCollisions(float dt)
  {
    for(int i = 0; i < 10; ++i)
    {
      for(auto& rit : rigid_bodies_)
      {
        rit->GetConstraints().Update(dt);
      }
      PhysicsScene::resolution_iteration_ = !PhysicsScene::resolution_iteration_; 
    }
    
    /*
    for each contact group
      resolve the first collider
      then resolve all of the colliders attached
    */
  }

  void PhysicsScene::UnloadScene()
  {
    scene_loaded_ = false;
    rigid_bodies_.clear();
    colliders_.clear();
  }

  void PhysicsScene::LoadScene()
  {
    Physics* pSys = (Physics*)Engine::GetCore()->GetSystem(ST_Physics);
    
    std::set<PhysicsComponent*>& comps = pSys->GetComponents();
    for(auto& comp : comps)
    {
      AddPhysicsObject(comp);
      Transform* tr = (Transform*)comp->GetSibling(CT_Transform);
      comp->SetPosition(tr->GetPosition());
    }

    scene_loaded_ = true;

  }

  void PhysicsScene::SetLoaded(bool loaded) 
  { 
    scene_loaded_ = loaded; 
  }

  void PhysicsScene::AddPhysicsObject(PhysicsComponent* comp)
  {
    rigid_bodies_.push_back(comp->GetRigidBody());
    auto bodyColliders = comp->GetRigidBody()->GetColliders();
    colliders_.insert(colliders_.end(), bodyColliders.begin(), bodyColliders.end());
  }

  void PhysicsScene::Refresh()
  {

  }

  void PhysicsScene::AddCollider(Collider* coll)
  {
    colliders_.push_back(coll);
  }

  std::vector<RigidBody*>& PhysicsScene::GetRigidBodies() 
  { 
    return rigid_bodies_; 
  }

  BroadPhase * PhysicsScene::GetDynamicBroadphase() 
  { 
    return dynamic_broadphase_; 
  }

  BroadPhase * PhysicsScene::GetStaticBroadphase() 
  { 
    return static_broadphase_; 
  }

  bool PhysicsScene::GetIteration() 
  { 
    return resolution_iteration_; 
  }
}
