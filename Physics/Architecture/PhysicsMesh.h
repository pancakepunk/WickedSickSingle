#pragma once
#include "AABB.h"

namespace WickedSick
{
  namespace PhysicsMeshType
  {
    enum Enum
    {
      AaBb,
      Sphere,
      Capsule,
      Custom,
      Count
    };
  }

  class PhysicsMesh
  {
    public:

      
      PhysicsMesh(const std::string& name, 
                  PhysicsMeshType::Enum type = PhysicsMeshType::Count);
      void InitializeMesh(const std::vector<Vector3>& mesh = std::vector<Vector3>(), 
                          float radius = 0.0f);
      std::vector<Vector3>& GetMesh();
      PhysicsMeshType::Enum& GetType();
      std::string GetName();
      float GetRadius();
      AABB& GetBoundingBox(){return bounding_box_;}
    private:

      std::vector<Vector3>  mesh_;
      AABB                  bounding_box_; //all has this
      float                 radius_;
      PhysicsMeshType::Enum type_;
      std::string           name_;
  };
}

