#pragma once

#include "GraphicsAPI.h"

#include "D3D/DXIncludes.h"
#include "Model/DxModel.h"
#include "Shader/DxShader.h"
#include "Texture/DxTexture.h"
#define D3D_DEBUG_INFO
namespace WickedSick
{

  struct Device;
  struct SwapChain;
  class Model;
  class Shader;
  class DirectX : public GraphicsAPI
  {
  public:
    DirectX();
    ~DirectX();

    void Initialize(GraphicsOptions* options,
                    Window* window);
    void CleanUp();
    void PrepareDebug(Buffer*& debugBuffer, Vertex* verts, size_t size) override;
    void RenderDebug(Buffer* debugBuffer) override;
    
    Model* MakeModel();
    Texture* MakeTexture(const std::string& name);
    Shader* MakeShader(const std::string& name, Shader::ShaderCallback callback, bool indexed = true);

    void BeginScene();
    void EndScene();
    
    SwapChain* GetSwapChain();

    
    
  private:
    
    

    void clear_buffers();
 
    SwapChain*                swap_chain_;
    Factory<DxModel>          model_factory_;
    Factory<DxShader>         shader_factory_;
    Factory<DxTexture>        texture_factory_;

    //grody dx stuff
    ID3D11RenderTargetView* back_buffer_;
    ID3D11DepthStencilState* depth_stencil_state_;
    ID3D11Texture2D* depth_stencil_buffer_;
    ID3D11DepthStencilView* depth_stencil_view_;
    ID3D11RenderTargetView* render_target_view_;
    ID3D11RasterizerState* rasterizer_state_;
    
  };
}