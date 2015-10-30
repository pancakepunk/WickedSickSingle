#include "GraphicsPrecompiled.h"

#include "Model.h"

#include "Math/MathInterface.h"

#include "Components/ModelComponent.h"
#include "Debugging/DebugInterface.h"

#include "Buffer/DxBuffer.h"

#include <fstream>


namespace WickedSick
{
  Model::Model()
  {
  }

  Model::~Model()
  {

  }

  void Model::Set(const std::vector<Vertex>& vertexList,
                  const std::vector<Face>&   faceList)  
  {
    vertex_list_ = vertexList;
    face_list_ = faceList;
  }

  int Model::GetNumIndices()
  {
    return face_list_.size() * 3;
  }

  int Model::GetNumFaces()
  {
    return face_list_.size();
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

  std::vector<Face>& Model::GetFaces()
  {
    return face_list_;
  }

  std::vector<ModelComponent*>& Model::GetInstances()
  {
    return instance_list_;
  }

  void Model::MapTextureCoords(MappingType::Enum type)
  {
    switch(type)
    {
      case MappingType::Planar:
      {
        Vector3 minimum( FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3 maximum = -minimum;
        for(auto& it : vertex_list_)
        {
          if(it.position.x < minimum.x){minimum.x = it.position.x;}
          if(it.position.y < minimum.y){minimum.y = it.position.y;}
          if(it.position.z < minimum.z){minimum.z = it.position.z;}

          if(it.position.x > maximum.x){maximum.x = it.position.x;}
          if(it.position.y > maximum.y){maximum.y = it.position.y;}
          if(it.position.z > maximum.z){maximum.z = it.position.z;}
        }

        Vector3 fullDiff = maximum - minimum;
        float largestGap = std::max(fullDiff.x, fullDiff.y);
        Vector3 diff;
        for(auto& it : vertex_list_)
        {
          diff = it.position - minimum;
          diff.x /= largestGap;
          diff.y /= largestGap;
          it.tex.x = diff.x;
          it.tex.y = -diff.y;
        }
        break;
      }
      case MappingType::Cylindrical:
      {
        Vector3 minimum(FLT_MAX, FLT_MAX, FLT_MAX);
        Vector3 maximum = -minimum;
        for(auto& it : vertex_list_)
        {
          if(it.position.x < minimum.x)
          {
            minimum.x = it.position.x;
          }
          if(it.position.y < minimum.y)
          {
            minimum.y = it.position.y;
          }
          if(it.position.z < minimum.z)
          {
            minimum.z = it.position.z;
          }

          if(it.position.x > maximum.x)
          {
            maximum.x = it.position.x;
          }
          if(it.position.y > maximum.y)
          {
            maximum.y = it.position.y;
          }
          if(it.position.z > maximum.z)
          {
            maximum.z = it.position.z;
          }
        }

        Vector3 fullDiff = maximum - minimum;
        float largestGap = std::max(fullDiff.x, fullDiff.y);
        Vector3 diff;
        for(auto& it : vertex_list_)
        {
          diff = it.position - minimum;
          diff.x /= largestGap;
          diff.y /= largestGap;
          it.tex.x = diff.x;
          it.tex.y = -diff.y;
        }
        break;
      }
      default:
        break;
    }

    if(buffers_.vertBuf || buffers_.indexBuf)
    {
      ReInitBuffers();
    }
    

    

  }

}