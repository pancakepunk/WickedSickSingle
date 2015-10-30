#pragma once
#include "Utility/UtilityInterface.h"
#include "Shader/ParamTypeInfo.h"
#include "General/GraphicsTypes.h"

namespace WickedSick
{

  struct ParamTypeInfo;
  class Buffer
  {
  public:

    Buffer(const std::string& name,
           size_t stride,
           size_t offset,
           void* init,
           size_t size,
           BufferType::Enum  type,
           AccessType::Enum accessType,
           UsageType::Enum usageType,
           ShaderType::Enum shaderType = ShaderType::Vertex);
    Buffer(const Buffer& buffer);
    virtual ~Buffer();

    virtual void* BufferPointer() = 0;
    virtual void* GetData() final;
    virtual void Initialize() = 0;
    virtual void ClearBuffer() = 0;
    

    size_t GetSize();
    virtual std::string GetName() final;
    virtual size_t GetStride() final;
    virtual size_t GetOffset() final;
    void AddParameter(ParamTypeInfo&& param);
    void AddParameter(const ParamTypeInfo& param);
    void SetParameter(const std::string& name, void* data);
    virtual std::unordered_map<std::string, ParamTypeInfo>& GetParameters();

  protected:

    std::string name_;

    size_t  stride_;
    size_t  offset_;

    void*   data_;
    size_t  size_;
    size_t  buf_num_;
    BufferType::Enum type_;
    AccessType::Enum access_type_;
    UsageType::Enum  usage_type_;


    

    std::unordered_map<std::string, ParamTypeInfo> parameters_;
  };
}