#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "System/Graphics.h"
#include "BinLoader.h"

#include "Model/Model.h"
#include "Model/Vertex.h"
#include "Model/Face.h"

namespace WickedSick
{
  BinLoader::BinLoader()
  {
  }

  BinLoader::~BinLoader()
  {
  }

  Model* BinLoader::Load(const std::string & source)
  {


    std::fstream modelFile(source, std::ios::in | std::ios::binary);


    Model* newModel = Graphics::graphicsAPI->MakeModel();

    std::vector<Vertex> outVerts;

    int vertCount;

    if (modelFile.is_open())
    {
      modelFile.read((char*)&vertCount, sizeof(vertCount));
      outVerts.resize(vertCount);
      
      modelFile.read((char*)&outVerts[0], vertCount * sizeof(WickedSick::Vertex));
  
    }


    newModel->Set(outVerts);
    return  newModel;
  }
  
}
