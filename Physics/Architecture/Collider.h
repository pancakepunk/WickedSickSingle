#pragma once

#include "AABB.h"

namespace WickedSick
{
  enum Axis
  {
    A_X,
    A_Y,
    A_Z,
    A_NEGX,
    A_NEGY,
    A_NEGZ,
    A_COUNT
  };
  struct PhysicsMaterial;
  class RigidBody;
  class PhysicsMesh;
  class ContactEdge;

  class Collider
  {
    public:

      Collider(RigidBody* base,
               PhysicsMaterial* material = nullptr,
               PhysicsMesh* mesh = nullptr,
               Vector3 half_extents = Vector3(1.0f, 1.0f, 1.0f),
               unsigned id = 0,
               Matrix3 orientation_matrix = Matrix3(),
               bool resolves = true,
               float volume = 1.0f);

      void                      SetUpConstraint();
      std::vector<Collider*>    GetCollisions();
      
      void                      RemoveEdgeWith(Collider* other);
      ContactEdge*              AddEdgeWith(Collider* other);
      ContactEdge*              GetEdgeWith(Collider* other);
      void                      clone(const Collider& rhs);
      void                      GenerateBoundingBox();

      unsigned                   GetId();
      float                      GetVolume();
      Vector3                    GetPosition();
      Matrix3                    GetOrientationMatrix();
      PhysicsMaterial*           GetMaterial();
      PhysicsMesh*               GetMesh();
      RigidBody*                 GetBase();
      std::vector<ContactEdge*>& GetEdges();

      bool Resolves();

      void SetId(unsigned set);
      void AddCollidingID(unsigned id);
      void RemoveCollidingID(unsigned id);
      std::set<unsigned>& GetCollidingIds();
      void ClearCollidingIDs();
      void SetVolume(float set);
      void SetPosition(Vector3 set);
      void SetOrientationMatrix(Matrix3 set);
      void SetMaterial(PhysicsMaterial* set);
      void SetMesh(PhysicsMesh* set);
      void SetBase(RigidBody* set);
      void SetEdges(std::vector<ContactEdge*> set);
      void SetResolve(bool resolve);

    private:

      std::string               name_;
      unsigned                  id_;
      std::set<unsigned>        colliding_ids_;
      float                     volume_;
      Vector3                   position_;            //get tr pos
      Matrix3                   orientation_matrix_;
      PhysicsMaterial*          material_;
      PhysicsMesh*              mesh_;                //get box from here
      RigidBody*                base_;
      bool                      resolves_;
      std::vector<ContactEdge*> edges_;
      
      bool draw_debug_;

      float get_extent(Axis axis);
    };
 
}
