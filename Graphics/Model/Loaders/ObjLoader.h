#pragma once

#include "ModelLoader.h"
#include "Model/Vertex.h"
#include "Model/Face.h"

namespace WickedSick
{
  struct ObjFace
  {
    Vector3i indices;
    Vector3i normals;
    Vector3i uvs;
  };

  class ObjLoader : public ModelLoader
  {
  public:
    ObjLoader();
    ~ObjLoader();
    Model* Load(const std::string& source);

  private:

    Vector3  extract_vertex(std::string target);
    Vector3  extract_normal(std::string target);
    ObjFace    extract_face  (std::string target);
    std::vector<Vector3> CalculateNormals(const std::vector<ObjFace>& faceList, const std::vector<Vector3>& vertexList);
  };
}
