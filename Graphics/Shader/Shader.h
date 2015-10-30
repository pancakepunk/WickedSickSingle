#pragma once
#include "ParamTypeInfo.h"

#include "Math/MathInterface.h"
#include "Utility/UtilityInterface.h"
#include "General/GraphicsTypes.h"

namespace WickedSick
{
  class ModelComponent;
  class Buffer;
  class Model;
  class Graphics;
  class Texture;

  class Shader
  {
    public:

      typedef void (*ShaderCallback)(Graphics*, void*, std::vector<ParamPasser>&);

      Shader(const std::string& name,
             ShaderCallback callback,
             bool indexed = true);
      ~Shader();

      virtual void Initialize() = 0;
      virtual bool Compile(bool forceCompile = false) = 0;
      virtual void ReflectBuffers() = 0;

      virtual void Render(int count) = 0;

      virtual void SetShaderDir(const std::string& dir) = 0;


      virtual void SetParameters(const std::vector<ParamPasser>& params) = 0;


      virtual Buffer* GetConstantBuffer(const std::string& name) final;
      virtual void AddConstantBuffer(Buffer* newBuf) final;
      virtual void AddInstance(ModelComponent* comp) final;
      virtual void ClearInstances() final;
      virtual std::unordered_map<Model*, std::vector<ModelComponent*>>& GetInstances() final;
      virtual std::string GetName() const final;
      virtual void PrepareBuffers(void* data = nullptr) final;

      virtual void SetIndexed(bool index) final;

      virtual void AddTexture(Texture* comp) final;
      virtual Texture* GetTexture(const std::string& name) final;
      virtual void ClearTextures() final;


    protected:
      //void OutputShaderErrorMessage(ID3D10Blob*, 
      //                              HWND, 
      //                              WCHAR*);
      std::string name_;
      ShaderCallback callback_;
      bool indexed_;//kinda shitty should change at some point

      std::unordered_map<Model*, std::vector<ModelComponent*>> instance_list_;

      std::unordered_map<std::string, Buffer*> constant_buffers_;

      std::vector<Texture*> textures_;


      FilePath files_[ShaderType::Count];

  };
}