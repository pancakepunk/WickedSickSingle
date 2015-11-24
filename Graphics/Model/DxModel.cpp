#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "DxModel.h"
#include "Buffer/DxBuffer.h"

#include "API/DirectX.h"
#include "D3D/SwapChain.h"
#include "D3D/Device.h"
#include "Texture/DxTexture.h"
#include "System/Graphics.h"


namespace WickedSick
{

  DxModel::DxModel()
  {

  }
  
  DxModel::~DxModel()
  {

  }
  
  void DxModel::Initialize()
  {
    //MapTextureCoords(MappingType::Spherical);
    buffers_.vertBuf = new DxBuffer("Vertex",
                                    sizeof(Vertex),
                                    0,
                                    &vertex_list_[0],
                                    sizeof(Vertex) * GetNumVerts(),
                                    WickedSick::BufferType::Vertex,
                                    AccessType::None,
                                    UsageType::Default);

    buffers_.vertBuf->Initialize();


  }
  
  void DxModel::Render()
  {
    DirectX* dx = (DirectX*)Graphics::graphicsAPI;
    ID3D11DeviceContext* context = dx->GetSwapChain()->device->D3DContext;

    DxBuffer* vert = (DxBuffer*) buffers_.vertBuf;

    ID3D11Buffer* vertBuf = (ID3D11Buffer*)vert->BufferPointer();

    
    // Set the vertex buffer to active in the input assembler so it can be rendered.
    context->IASetVertexBuffers(0, 
                                1, 
                                &vertBuf, 
                                &vert->stride_, 
                                &vert->offset_);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
    context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    
  }

  void DxModel::ReInitBuffers()
  {
    if(buffers_.vertBuf)
    {
      buffers_.vertBuf->ClearBuffer();
      delete buffers_.vertBuf;
    }
    buffers_.vertBuf = new DxBuffer("Vertex",
                                    sizeof(Vertex),
                                    0,
                                    &vertex_list_[0],
                                    sizeof(Vertex) * GetNumVerts(),
                                    WickedSick::BufferType::Vertex,
                                    AccessType::None,
                                    UsageType::Default);




    buffers_.vertBuf->Initialize();


  }
  
}
