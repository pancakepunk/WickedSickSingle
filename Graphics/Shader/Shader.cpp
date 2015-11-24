#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "Shader.h"


//temp
#include "D3D/DXIncludes.h"
#include "API/DirectX.h"
#include "System/Graphics.h"
#include "D3D/SwapChain.h"
#include "D3D/Device.h"
#include "Buffer/Buffer.h"
#include "Graphics/Texture/Texture.h"


namespace WickedSick
{
  Shader::Shader(const std::string& name,
                 ShaderCallback callback)
  : name_(name),
    callback_(callback)
  {
  
    files_[ShaderType::Vertex] = "Content/Shaders/" + name_ + "/" + name_ + ".vs";
    files_[ShaderType::Pixel] = "Content/Shaders/" + name_ + "/" + name_ + ".ps";

  }

  Shader::~Shader()
  {
  }

  Buffer * Shader::GetConstantBuffer(const std::string & name)
  {
    auto it = constant_buffers_.find(name);
    if(it != constant_buffers_.end())
    {
      return (*it).second;
    }
    return nullptr;
  }

  void Shader::AddConstantBuffer(Buffer * newBuf)
  {
    constant_buffers_.insert(std::make_pair(newBuf->GetName(), newBuf));
  }

  void Shader::AddInstance(ModelComponent* comp)
  {
    auto it = instance_list_.find(comp->GetBase());
    if(it != instance_list_.end())
    {
      it->second.push_back(comp);
    }
    else
    {
      instance_list_.insert(std::make_pair(comp->GetBase(), std::vector<ModelComponent*>{comp}));
    }
    
  }

  void Shader::ClearInstances()
  {
    instance_list_.clear();
  }

  std::unordered_map<Model*, std::vector<ModelComponent*>>& Shader::GetInstances()
  {
    return instance_list_;
  }

  std::string Shader::GetName() const
  {
    return name_;
  }

  void Shader::PrepareBuffers(void* data)
  {
    Graphics* graphics = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    
    
    std::vector<ParamPasser> params;
    (*callback_)(graphics, data, params);
    SetParameters(params);
    for(auto& it : params)
    {
      it.Release();
    }
    ClearTextures();
  }

  void Shader::AddTexture(Texture * comp)
  {
    if(comp)
    {
      for(size_t i = 0; i < textures_.size(); ++i)
      {
        if(textures_[i]->GetName() == comp->GetName())
        {
          textures_[i] = comp;
          return;
        }
      }
      textures_.push_back(comp);
    }
  }

  Texture * Shader::GetTexture(const std::string & name)
  {
    for(size_t i = 0; i < textures_.size(); ++i)
    {
      if(textures_[i]->GetName() == name)
      {
        return textures_[i];
      }
    }
    
    return nullptr;
  }

  void Shader::ClearTextures()
  {
    textures_.clear();
  }

}