#include "Precompiled.h"
#include "GraphicsPrecompiled.h"

#include "System/Graphics.h"
#include "Model/Model.h"
#include "Utility/UtilityInterface.h"

#include "ObjLoader.h"



namespace WickedSick
{

  ObjLoader::ObjLoader()
  {

  }

  ObjLoader::~ObjLoader()
  {

  }

  Model* ObjLoader::Load(const std::string& source)
  {
    Model* newModel = nullptr;


    std::vector<Vertex> vertexList;
    std::vector<int> indexList;
    std::vector<Face> faceList;
    std::vector<Vector3> objVertexList;
    std::vector<ObjFace> objFaceList;
    std::vector<Vector3> objNormalList;
    //std::vector<Vector2> objUvList;

    int normals = 0;
    std::ifstream file(source, std::ifstream::beg | std::ifstream::in);
    //
    if (file.is_open())
    {
      std::string buffer;
      while (!file.eof())
      {
        std::getline(file, buffer);
        if (!buffer.empty())
        {
          int firstSpace = buffer.find_first_of(' ');
          std::string lineSig = buffer.substr(0, firstSpace - 0);
          if (lineSig.find("#") != std::string::npos)
          {
            continue;
          }
          buffer = buffer.substr(lineSig.length(), buffer.size() - lineSig.length());
          if (lineSig == "f")
          {
            objFaceList.push_back(extract_face(buffer));
          }
          else if (lineSig == "v")
          {
            objVertexList.push_back(extract_vertex(buffer));
          }
          else if (lineSig == "vn")
          {
            objNormalList.push_back(extract_normal(buffer));
            ++normals;
          }
          else if (lineSig == "g")
          {

          }
          else if (lineSig == "vt")
          {

          }
          else if (lineSig == "vp")
          {

          }
          else if (lineSig == "#" || lineSig == "\n" || lineSig == "")
          {
            // do nothing
          }
          else
          {
            ConsolePrint("Invalid model file (" + source + ")");
          }
        }
      }
      std::vector<Vector3> calculatedNormals;
      calculatedNormals.reserve(objFaceList.size());
      if (objNormalList.empty())
      {
        calculatedNormals = CalculateNormals(objFaceList, objVertexList);
      }


      newModel = Graphics::graphicsAPI->MakeModel();

      
      indexList.reserve(objFaceList.size() * 3);
      vertexList.reserve(objFaceList.size() * 3);
      HashMap<Vertex, int> vertMap(objFaceList.size() * 6);
      Vertex vert;
      int i;
      HashMap<Vertex, int>::Iterator vertIt;
      for (size_t whichFace = 0; whichFace < objFaceList.size(); ++whichFace)//auto& it : objFaceList)
      {
        for (i = 0; i < 3; ++i)
        {
          vert.position = objVertexList[objFaceList[whichFace].indices[i]];
          if (objNormalList.empty())
          {
            vert.normal = calculatedNormals[whichFace];
          }
          else
          {
            vert.normal = objNormalList[objFaceList[whichFace].normals[i]];
          }
          
          //vert.tex = (objUvList.empty()) ? Vector2() : objUvList[it.uvs[i]];
          vertIt = vertMap.find(vert);
          if (vertIt == vertMap.end())
          {
            indexList.push_back(vertexList.size());
            vertexList.push_back(vert);
            vertMap.insert(vert, indexList.back());
          }
          else
          {
            indexList.push_back((*vertIt).val);
          }
        }
      }

      

      Face face;
      for (size_t i = 0; i < indexList.size(); i += 3)
      {
        face.indices[0] = indexList[i    ];
        face.indices[1] = indexList[i + 1];
        face.indices[2] = indexList[i + 2];
        
        Vector4 v1v0 = vertexList[face.indices[1]].position - vertexList[face.indices[0]].position;
        Vector4 v2v0 = vertexList[face.indices[2]].position - vertexList[face.indices[0]].position;
        Vector4 faceNormal = v1v0.Cross(v2v0);
        //face.normal = faceNormal;

        faceList.push_back(face);
      }

      newModel->Set(vertexList);
    }
    else
    {
      ConsolePrint("Model file (" + source + ") not found.");
    }

    return newModel;
  }

  Vector3 ObjLoader::extract_vertex(std::string target)
  {
    int index = target.find_first_not_of(' ');
    target = target.substr(index, target.size() - index);
    target = TrimSpaces(target);
    Vector3 toInsert;
    std::string entries[3];
    int curString = 0;
    for(unsigned i = 0; i < target.size(); ++i)
    {
      if(target[i] == ' ')
      {
        ++curString;
      }
      else
      {
        entries[curString].push_back(target[i]);
      }
    }

    toInsert[0] = static_cast<float>(::atof(entries[0].c_str()));
    toInsert[1] = static_cast<float>(::atof(entries[1].c_str()));
    toInsert[2] = static_cast<float>(::atof(entries[2].c_str()));

    return toInsert;
  }

  Vector3 ObjLoader::extract_normal(std::string target)
  {

    int index = target.find_first_not_of(' ');
    target = target.substr(index, target.size() - index);
    target = TrimSpaces(target);
    Vector3 toInsert;

    int innerIndex = target.find_first_of(' ') + 1;
    int innerIndex2 = target.find_last_of(' ');
    std::string first = target.substr(0, (innerIndex == std::string::npos) ? target.size() : innerIndex);




    std::string second = target.substr((innerIndex == std::string::npos) ? 0 : innerIndex, (innerIndex2 == std::string::npos) ? target.size() : (target.size() - innerIndex2));
    std::string third = target.substr((innerIndex2 == std::string::npos) ? 0 : innerIndex2, target.size());
    toInsert[0] = static_cast<float>(::atof(first.c_str()));
    toInsert[1] = static_cast<float>(::atof(second.c_str()));
    toInsert[2] = static_cast<float>(::atof(third.c_str()));

    return toInsert;
  }

  ObjFace ObjLoader::extract_face(std::string target)
  {
    int index = target.find_first_not_of(' ');
    target = target.substr(index, target.size() - index);

    target = TrimSpaces(target);
    
    ObjFace toReturn;
    std::string floatBuf;
    for (int i = 0; i < 3; ++i)
    {
      index    = target.find_first_of(' ');
      floatBuf    = target.substr(0, index - 0);

      int innerIndex = floatBuf.find_first_of('/');
      int innerIndex2 = floatBuf.find_last_of('/');
      std::string first = floatBuf.substr(0, (innerIndex == std::string::npos) ? floatBuf.size() : innerIndex);
      std::string second = floatBuf.substr((innerIndex == std::string::npos) ? 0 : innerIndex + 1, (innerIndex2 == std::string::npos) ? floatBuf.size() : (floatBuf.size() - (innerIndex2 - 1)));
      std::string third = floatBuf.substr((innerIndex2 == std::string::npos) ? 0 : innerIndex2 + 1, floatBuf.size());
      
      // -1 here because obj is dumb and counts from 1
      toReturn.indices[i] = ::atoi(first.c_str()) - 1;
      toReturn.uvs[i] = ::atoi(second.c_str()) - 1;
      toReturn.normals[i] = ::atoi(third.c_str()) - 1;

      index += 1;
      index = clamp(index, 0, (int)target.size());
      target = target.substr(index, target.size() - index);
    }
    return toReturn;
  }

  std::vector<Vector3> ObjLoader::CalculateNormals( const std::vector<ObjFace>& faceList,
                                                    const std::vector<Vector3>& vertexList)
  {
    std::vector<Vector3> normals;
    for (auto& it : faceList)
    {
      Vector3 v1v0 = vertexList[it.indices[1]] - vertexList[it.indices[0]];
      v1v0.Normalize();
      Vector3 v1v2 = vertexList[it.indices[2]] - vertexList[it.indices[0]];
      v1v0.Normalize();
      Vector3 faceNormal = v1v0.Cross(v1v2);

      normals.push_back(faceNormal.GetNormalized());
    }
    return normals;
  }

  //void ObjLoader::SmoothNormals()
  //{
  //  HashMap<Vector4, std::vector<Vector3*>> normalMap;
  //  for (auto& it : face_list_)
  //  {
  //    for (int i = 0; i < 3; ++i)
  //    {
  //      auto& mIt = normalMap.find(vertex_list_[it.indices[i]].position);
  //      if (mIt != normalMap.end())
  //      {
  //        (*mIt).val.push_back(&vertex_list_[it.indices[i]].normal);
  //      }
  //    }
  //  }
  //
  //  for (auto& it : normalMap)
  //  {
  //    Vector3 normal;
  //    int numNormals;
  //    for (int i = 0; i < it.val.size(); ++i)
  //    {
  //      normal += *it.val[i];
  //    }
  //    normal /= it.val.size();
  //    normal.Normalize();
  //    for (int i = 0; i < it.val.size(); ++i)
  //    {
  //      *it.val[i] = normal;
  //    }
  //  }
  //}

}
