#pragma once

#include "Math/MathInterface.h"
#include "ModelInstance.h"

#include "Renderable.h"

#include "Utility/UtilityInterface.h"
#include "Face.h"
#include "API/APIType.h "

//

namespace WickedSick
{
  namespace MappingType
  {
    enum Enum
    {
      Planar,
      Cylindrical,
      Spherical,
      Cubic,
      Count
    };
  }

  namespace VertexType
  {
    enum Enum
    {
      IndexedTri,
      TriList,
      IndexedLine,
      LineList,
      PointList,
      Count
    };
  }

  class Texture;
  class Buffer;
  class ModelComponent;
  class Model
  {
  public:
    Model();
    virtual ~Model();

    virtual void Initialize() = 0;

    void Set(const std::vector<Vertex>& vertexList);

    int GetNumIndices();
    int GetNumFaces();
    int GetNumVerts();

    void AddInstance(ModelComponent* inst);
    void RemoveInstance(ModelComponent* inst);
    std::vector<Vertex>& GetVerts();
    virtual void Render() = 0;

    std::vector<ModelComponent*>& GetInstances();

    void ComputeTangents(MappingType::Enum type);

    virtual void ReInitBuffers() = 0;

  protected:

    Renderable buffers_;
    friend class DirectX;

    std::vector<ModelComponent*> instance_list_;
    std::vector<Vertex> vertex_list_;

  };
}
