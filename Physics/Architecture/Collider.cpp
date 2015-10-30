#include "PhysicsPrecompiled.h"
#include "Collider.h"

#include "Resolution/Contact.h"
#include "Resolution/ContactEdge.h"
#include "Resolution/ContactManifold.h"


#include "PhysicsScene.h"
#include "Components/PhysicsComponent.h"
#include "PhysicsMesh.h"
#include "PhysicsMaterial.h"
#include "RigidBody.h"


namespace WickedSick
{
  Collider::Collider(RigidBody* base,
                     PhysicsMaterial* material,
                     PhysicsMesh* mesh,
                     Vector3,
                     unsigned,
                     Matrix3 orientation_matrix,
                     bool resolves, 
                     float volume) 
                      : base_(base),
                        material_(material),
                        mesh_(mesh),
                        orientation_matrix_(orientation_matrix),
                        volume_(volume),
                        resolves_(resolves),
                        draw_debug_(false)
  {
    if(mesh_)
    {
      GenerateBoundingBox();
    }
  }

  void Collider::clone(const Collider& rhs)
  {
    //generate id?
    resolves_ = rhs.resolves_;
    volume_ = rhs.volume_;
    id_ = rhs.id_;
    colliding_ids_ = rhs.colliding_ids_;
    position_ = rhs.position_;
    orientation_matrix_ = rhs.orientation_matrix_;
    material_ = rhs.material_;
    mesh_ = rhs.mesh_;
    //base should be set
    //edges are generated data
  }

  void Collider::RemoveEdgeWith(Collider* other)
  {
    ContactEdge* blah = GetEdgeWith(other);
    for(unsigned i = 0; i < edges_.size(); ++i)
    {
      if(edges_[i]->GetOther() == other)
      {
        for(unsigned j = 0; j < edges_[i]->GetOther()->GetEdges().size(); ++j)
        {
          if(edges_[i]->GetOther()->GetEdges()[j]->GetOther() == this)
          {
            ContactEdge* todelete = edges_[i]->GetOther()->GetEdges()[j];
            vector_remove(edges_[i]->GetOther()->GetEdges(), j);
            delete todelete;
            break;
          }
        }
        ContactEdge* todelete = edges_[i];
        vector_remove(edges_, i);
        delete todelete->GetManifold();
        delete todelete;
        return;
      }
    }
  }

  std::vector<Collider*> Collider::GetCollisions()
  {
    std::vector<Collider*> collisions;
    for(auto it : edges_)
    {
      collisions.push_back(it->GetOther());
    }
    return collisions;
  }

  ContactEdge* Collider::AddEdgeWith(Collider* other)
  {
    if(GetEdgeWith(other))
    {
      return nullptr;
    }
    edges_.push_back(new ContactEdge(this, other));
    other->edges_.push_back(new ContactEdge(other, this));

    edges_.back()->SetManifold(new ContactManifold(this, other));
    other->edges_.back()->SetManifold(edges_.back()->GetManifold());
    
    return edges_.back();
  }

  ContactEdge* Collider::GetEdgeWith(Collider* other)
  {
    for(auto it : edges_)
    {
      if(it->GetOther() == other)
      {
        return it;
      }
    }
    return nullptr;
  }

  void Collider::SetId(unsigned set) 
  { 
    id_ = set; 
  }

  void Collider::AddCollidingID(unsigned id)
  {
    colliding_ids_.insert(id);
  }

  void Collider::RemoveCollidingID(unsigned id)
  {
    colliding_ids_.erase(id);
  }

  std::set<unsigned>& Collider::GetCollidingIds() 
  { 
    return colliding_ids_; 
  }

  void Collider::ClearCollidingIDs()
  {
    colliding_ids_.clear();
  }

  void Collider::SetVolume(float set) 
  { 
    volume_ = set; 
  }

  void Collider::SetPosition(Vector3 set) 
  { 
    position_ = set; 
  }

  void Collider::SetOrientationMatrix(Matrix3 set) 
  { 
    orientation_matrix_ = set; 
  }

  void Collider::SetMaterial(PhysicsMaterial * set) 
  { 
    material_ = set; 
  }

  void Collider::SetMesh(PhysicsMesh * set) 
  { 
    mesh_ = set; 
  }

  void Collider::SetBase(RigidBody * set) 
  { 
    base_ = set; 
  }

  void Collider::SetEdges(std::vector<ContactEdge*> set) 
  { 
    edges_ = set; 
  }

  void Collider::SetResolve(bool resolve) 
  { 
    resolves_ = resolve; 
  }

  void Collider::GenerateBoundingBox()
  {
    mesh_->InitializeMesh();
  }

  unsigned Collider::GetId() 
  { 
    return id_; 
  }

  float Collider::GetVolume() 
  { 
    return volume_; 
  }

  Vector3 Collider::GetPosition() 
  { 
    return position_; 
  }

  Matrix3 Collider::GetOrientationMatrix() 
  { 
    return orientation_matrix_; 
  }

  PhysicsMaterial * Collider::GetMaterial() 
  { 
    return material_; 
  }

  PhysicsMesh * Collider::GetMesh() 
  { 
    return mesh_; 
  }

  RigidBody * Collider::GetBase() 
  { 
    return base_; 
  }

  std::vector<ContactEdge*>& Collider::GetEdges() 
  { 
    return edges_; 
  }

  bool Collider::Resolves() 
  { 
    return resolves_; 
  }

  float Collider::get_extent(Axis axis)
  {
    switch(mesh_->GetType())
    {
      case PhysicsMeshType::Custom:
      {
        float farthest = 0.0f;
        switch(axis)
        {
          case A_X:
            for (auto it : mesh_->GetMesh())
            {
              if(it.x > farthest)
              {
                farthest = it.x;
              }
            }
            break;
          case A_NEGX:
            for (auto it : mesh_->GetMesh())
            {
              if(it.x < farthest)
              {
                farthest = it.x;
              }
            }
            break;
          case A_Y:
            for (auto it : mesh_->GetMesh())
            {
              if(it.y > farthest)
              {
                farthest = it.y;
              }
            }
            break;
          case A_NEGY:
            for (auto it : mesh_->GetMesh())
            {
              if(it.y < farthest)
              {
                farthest = it.y;
              }
            }
            break;
          case A_Z:
            for (auto it : mesh_->GetMesh())
            {
              if(it.z > farthest)
              {
                farthest = it.z;
              }
            }
            break;
          case A_NEGZ:
            for (auto it : mesh_->GetMesh())
            {
              if(it.z < farthest)
              {
                farthest = it.z;
              }
            }
            break;
        }
        return farthest;
      }
      case PhysicsMeshType::Sphere:
      {
        if(axis < A_NEGX)
        {
          return mesh_->GetRadius();
        }
        return -mesh_->GetRadius();
      }
      case PhysicsMeshType::Capsule:
      {
        if(axis < A_NEGX)
        {
          if(axis == A_Y)
          {
            float halfHeight = abs(mesh_->GetMesh().front().y) + abs(mesh_->GetMesh().back().y);
            halfHeight *= 0.5f;
            halfHeight += mesh_->GetRadius();
            return halfHeight;
          }
          return mesh_->GetRadius();
        }
        if(axis == A_NEGY)
        {
          float halfHeight = abs(mesh_->GetMesh().front().y) + abs(mesh_->GetMesh().back().y);
          halfHeight *= 0.5f;
          halfHeight += mesh_->GetRadius();
          return -halfHeight;
        }
        return -mesh_->GetRadius();
      }
    }
    return 0;
  }

}

