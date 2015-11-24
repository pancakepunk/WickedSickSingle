#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "Buffer.h"

namespace WickedSick
{
  Buffer::Buffer(const std::string& name, 
                 size_t stride,
                 size_t offset,
                 void* init,
                 size_t size,
                 BufferType::Enum  type,
                 AccessType::Enum accessType,
                 UsageType::Enum usageType,
                 ShaderType::Enum shaderType)
                  : name_(name),
                    stride_(stride),
                    offset_(offset),
                    data_(nullptr),
                    size_(size),
                    type_(type),
                    access_type_(accessType),
                    usage_type_(usageType)
  {
    if(data_)
    {
      delete[] data_;
    }
    data_ = (void*)new char[size_];
    if(init)
    {
      memcpy(data_, init, size_);
    }
    else
    {
      memset(data_, 0, size_);
    }
    
    
  }

  Buffer::Buffer( const Buffer& buffer)
                  : stride_(buffer.stride_),
                    offset_(buffer.offset_),
                    data_(nullptr),
                    size_(buffer.size_),
                    type_(buffer.type_),
                    access_type_(buffer.access_type_),
                    usage_type_(buffer.usage_type_)
  {
    if(data_)
    {
      delete[] data_;
    }
    data_ = (void*)new char[size_];
    if(buffer.data_)
    {
      memcpy(data_, buffer.data_, size_);
    }
    else
    {
      memset(data_, 0, size_);
    }
    
  }

  Buffer::~Buffer()
  {
    if(data_)
    {
      delete[] data_;
    }
  }

  void * Buffer::GetData()
  {
    return data_;
  }

  size_t Buffer::GetSize()
  {
    return size_;
  }

  std::string Buffer::GetName()
  {
    return name_;
  }

  size_t Buffer::GetStride()
  {
    return stride_;
  }

  size_t Buffer::GetOffset()
  {
    return offset_;
  }

  void Buffer::AddParameter(ParamTypeInfo && param)
  {
    parameters_.insert(std::make_pair(param.name, std::move(param)));
  }

  void Buffer::AddParameter(const ParamTypeInfo & param)
  {
    parameters_.insert(std::make_pair(param.name, param));
  }

  void Buffer::SetParameter(const std::string & name, void * data)
  {
    auto& it = parameters_.find(name);
    if(it != parameters_.end())
    {
      ParamTypeInfo& param = it->second;
      memcpy(reinterpret_cast<char*>(data_) + param.offset, data, param.size);
    }
  }

  std::unordered_map<std::string, ParamTypeInfo>& Buffer::GetParameters()
  {
    return parameters_;
  }

}
