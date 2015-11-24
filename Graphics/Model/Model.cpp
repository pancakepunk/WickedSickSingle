#include "Precompiled.h"
#include "GraphicsPrecompiled.h"

#include "Model.h"

#include "Math/MathInterface.h"

#include "Components/ModelComponent.h"
#include "Debugging/DebugInterface.h"

#include "Buffer/DxBuffer.h"




namespace WickedSick
{
  Model::Model()
  {
  }

  Model::~Model()
  {

  }

  void Model::Set(const std::vector<Vertex>& vertexList)  
  {
    vertex_list_ = vertexList;
  }


  int Model::GetNumFaces()
  {
    return vertex_list_.size() / 3;
  }

  int Model::GetNumVerts()
  {
    return vertex_list_.size();
  }

  void Model::AddInstance(ModelComponent* inst)
  {
    instance_list_.push_back(inst);
  }

  void Model::RemoveInstance(ModelComponent* inst)
  {
    for (size_t i = 0; i < instance_list_.size(); ++i)
    {
      if (instance_list_[i] == inst)
      {
        vector_remove(instance_list_, i);
        break;
      }
    }
  }

  std::vector<Vertex>& Model::GetVerts()
  {
    return vertex_list_;
  }


  std::vector<ModelComponent*>& Model::GetInstances()
  {
    return instance_list_;
  }


  Vector2 computeTexCylindrical(const Vertex& vert, const Vector3& minimum, const Vector3& maximum)
  {
    Vector2 tex;
    if((vert.position.x == vert.position.z) && (vert.position.x == 0.0f))
    {
      tex.x = 0.0f;
    }
    else
    {
      tex.x = (std::atan(vert.position.z / vert.position.x) + (PI / 2.0f)) / PI;
    }


    tex.y = (vert.position.y - minimum.y) / (maximum.y - minimum.y);
    return tex;
  }

  Vector2 computeTexSpherical(const Vertex& vert, const Vector3& minimum, const Vector3& maximum)
  {
    Vector2 tex;
    float r = vert.position.Length();
    
    if((vert.position.x == vert.position.z) && (vert.position.x == 0.0f))
    {
      tex.x = 0.0f;
    }
    else
    {
      tex.x = (std::atan(vert.position.z / vert.position.x) + (PI / 2.0f)) / PI;
      tex.y = std::acos(vert.position.y / r) / PI;
    }

    return tex;
  }

  void Model::ComputeTangents(MappingType::Enum type)
  {

  }

}