
#include "PhysicsPrecompiled.h"
#include "PhysicsMesh.h"

namespace WickedSick
{
  PhysicsMesh::PhysicsMesh(const std::string& name,
                           PhysicsMeshType::Enum type) 
                            : name_(name),
                              type_(type),
                              radius_(0)
  {
  }

  void PhysicsMesh::InitializeMesh(const std::vector<Vector3>& mesh, float radius)
  {
    float lenSq;
    //if we have an arbitrary mesh
    if (mesh.size())
    {
      radius_ = 0.0f;
      mesh_ = mesh;
      //loop through the mesh and find out extents of box and farthest point.
      for (auto& it : mesh)
      {

        //farthest point len will be radius
        lenSq = it.LengthSq();
        if (lenSq > radius_)
        {
          radius_ = lenSq;
        }

        //greatest x,y,z
        if (it.x > bounding_box_.max_.x)
        {
          bounding_box_.max_.x = it.x;
        }
        if (it.y > bounding_box_.max_.y)
        {
          bounding_box_.max_.y = it.y;
        }
        if (it.z > bounding_box_.max_.z)
        {
          bounding_box_.max_.z = it.z;
        }

        //least x,y,z
        if (it.x < bounding_box_.min_.x)
        {
          bounding_box_.min_.x = it.x;
        }
        if (it.y < bounding_box_.min_.y)
        {
          bounding_box_.min_.y = it.y;
        }
        if (it.z < bounding_box_.min_.z)
        {
          bounding_box_.min_.z = it.z;
        }
      }
      //save sqrt till end
      radius_ = sqrt(radius_);
    }
    else if (radius)
    {
      radius_ = radius;
      bounding_box_.max_.x = bounding_box_.max_.y = bounding_box_.max_.z = radius_;
      bounding_box_.min_.x = bounding_box_.min_.y = bounding_box_.min_.z = -radius_;
    }
    else
    {
      Vector3 radVecFromBox = bounding_box_.max_ - bounding_box_.min_;
      radVecFromBox *= 0.5f;
      radius_ = radVecFromBox.Length();
    }

  }

  std::vector<Vector3>& PhysicsMesh::GetMesh() 
  { 
    return mesh_; 
  }

  PhysicsMeshType::Enum & PhysicsMesh::GetType() 
  { 
    return type_; 
  }

  std::string PhysicsMesh::GetName() 
  { 
    return name_; 
  }

  float PhysicsMesh::GetRadius()
  {
    return radius_;
  }

}
