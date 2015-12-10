#pragma once
#include "Shader/ParamTypeInfo.h"
#include "Shader/Shader.h"
#include "General/GraphicsTypes.h"
#include "Texture/Texture.h"
#include "RenderTarget/RenderTarget.h"

namespace WickedSick
{
  namespace BlendType
  {
    enum Enum
    {
      Additive,
      Normal,
      Off,
      Count
    };
  }

  namespace DepthType
  {
    enum Enum
    {
      Normal,
      Off,
      Count
    };
  }
  class Model;
  class Shader;
  struct GraphicsOptions;
  class Window;
  class Shader;
  class Buffer;
  class RenderTarget;
  enum APIType::Enum;
  struct Vertex;

  class GraphicsAPI
  {
  public:

    GraphicsAPI(APIType::Enum api);
    virtual ~GraphicsAPI();

    virtual void Initialize(GraphicsOptions* options,
                            Window* window) = 0;
    virtual void CleanUp() = 0;

    virtual void PrepareDebug(Buffer*& debugBuffer, Vertex* verts, size_t size) = 0;
    virtual void RenderDebug(Buffer* debugBuffer) = 0;

    virtual void BeginScene() = 0;
    virtual void EndScene() = 0;

    virtual Model* MakeModel() = 0;
    virtual Texture* MakeTexture(const TextureDesc& desc) = 0;
    virtual Texture* MakeTexture(const std::string& name) = 0;
    virtual Texture* MakeTexture(const std::vector<unsigned char>& tex, 
                                 const TextureDesc& desc) = 0;
    virtual Shader* MakeShader(const std::string& name, Shader::ShaderCallback callback) = 0;
    virtual RenderTarget* MakeRenderTarget(const RenderTargetDesc& desc) = 0;

    virtual void AddRenderTarget(RenderTarget* target) final;
    virtual void SetRenderTargets() = 0;
    virtual void ClearRenderTargets() final;

    virtual void ClearShaderResources() = 0;

    virtual void FlushDepth() = 0;

    
    virtual void SetBlendType(BlendType::Enum type) = 0;
    virtual void SetDepthType(DepthType::Enum type) = 0;

  protected:
    GraphicsOptions* options_;

    std::vector<RenderTarget*> current_render_targets_;

    virtual void clear_buffers() = 0;
    APIType::Enum api_;

  };
}
