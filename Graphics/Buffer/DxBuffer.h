#pragma once
#include "Buffer.h"

struct ID3D11Device;
struct ID3D11Buffer;
namespace WickedSick
{
  
  class DxBuffer : public Buffer
  {
  public:
    DxBuffer(const std::string& name,
             size_t stride,
             size_t offset,
             void* init,
             size_t size,
             BufferType::Enum  type,
             AccessType::Enum accessType,
             UsageType::Enum usageType);
    DxBuffer(const DxBuffer& buffer);
    ~DxBuffer();
    
    void Initialize() override;
    void ClearBuffer() override;
    void* BufferPointer() override;
    void SetIndex(ShaderType::Enum type, size_t index);
    size_t GetIndex(ShaderType::Enum type);

    static const int invalidIndex = -1;
  private:

    friend class DirectX;
    friend class DxModel;

    size_t buffer_index_[ShaderType::Count];
    ID3D11Buffer* buffer_;
  };
}
