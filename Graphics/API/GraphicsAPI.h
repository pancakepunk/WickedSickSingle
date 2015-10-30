#pragma once
#include "Shader/ParamTypeInfo.h"
#include "Shader/Shader.h"
#include "General/GraphicsTypes.h"

namespace WickedSick
{
  
  class Model;
  class Shader;
  struct GraphicsOptions;
  class Window;
  class Shader;
  class Buffer;
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
    virtual Texture* MakeTexture(const std::string& name) = 0;
    virtual Shader* MakeShader(const std::string& name, Shader::ShaderCallback callback, bool indexed) = 0;
    
  protected:
    GraphicsOptions* options_;

    virtual void clear_buffers() = 0;
    APIType::Enum api_;

  };
}
