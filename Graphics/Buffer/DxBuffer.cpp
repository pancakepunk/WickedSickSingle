#include "GraphicsPrecompiled.h"
#include "DxBuffer.h"

#include "D3D/DXIncludes.h"

#include "API/DirectX.h"
#include "D3D/SwapChain.h"
#include "D3D/Device.h"

#include "System/Graphics.h"

#include "General/GraphicsUtility.h"



namespace WickedSick
{
  DxBuffer::DxBuffer(const std::string& name,
                     size_t stride,
                     size_t offset,
                     void* init,
                     size_t size,
                     BufferType::Enum  type,
                     AccessType::Enum accessType,
                     UsageType::Enum usageType)
                      : Buffer(name,
                               stride,
                               offset,
                               init,
                               size,
                               type,
                               accessType,
                               usageType)
  {
    for(size_t i = 0; i < ShaderType::Count; ++i)
    {
      buffer_index_[i] = invalidIndex;
    }
  }

  DxBuffer::~DxBuffer()
  {
    
  }
  
  void DxBuffer::Initialize()
  {
    DirectX* dx = (DirectX*)Graphics::graphicsAPI;
    ID3D11Device* device = dx->GetSwapChain()->device->D3DDevice;
    D3D11_BUFFER_DESC bufferDesc;
    SecureZeroMemory(&bufferDesc, sizeof(D3D11_BUFFER_DESC));
    D3D11_SUBRESOURCE_DATA* buffData = nullptr;

    
    bufferDesc.ByteWidth = size_;

    switch (type_)
    {
    case BufferType::Vertex:
      bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
      buffData = (D3D11_SUBRESOURCE_DATA*)alloca(sizeof(D3D11_SUBRESOURCE_DATA));
      SecureZeroMemory(buffData, sizeof(D3D11_SUBRESOURCE_DATA));
      buffData->pSysMem = data_;
      break;
    case BufferType::Index:
      bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
      buffData = (D3D11_SUBRESOURCE_DATA*)alloca(sizeof(D3D11_SUBRESOURCE_DATA));
      SecureZeroMemory(buffData, sizeof(D3D11_SUBRESOURCE_DATA));
      buffData->pSysMem = data_;
      break;
    case BufferType::Constant:
      bufferDesc.ByteWidth = size_ + (16 - (size_ % 16));
      bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
      break;
    default:
      break;
    }

    switch (access_type_)
    {
      case AccessType::CpuBoth:
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
        break;
      case AccessType::CpuRead:
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
        break;
      case AccessType::CpuWrite:
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        break;
      default:
        bufferDesc.CPUAccessFlags = 0;
        break;
    }

    switch (usage_type_)
    {
      case UsageType::Default:
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        break;
      case UsageType::Dynamic:
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        break;
      case UsageType::Static:
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        break;
      case UsageType::Staging:
        bufferDesc.Usage = D3D11_USAGE_STAGING;
        break;
      default:
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    }


    DxError(device->CreateBuffer(&bufferDesc, 
                                 buffData, 
                                 &buffer_));
  }

  void DxBuffer::ClearBuffer()
  {
    buffer_->Release();
    buffer_ = nullptr;
  }

  void* DxBuffer::BufferPointer()
  {
    return buffer_;
  }

  void DxBuffer::SetIndex(ShaderType::Enum type, size_t index)
  {
    buffer_index_[type] = index;
  }

  size_t DxBuffer::GetIndex(ShaderType::Enum type)
  {
    return buffer_index_[type];
  }

}
