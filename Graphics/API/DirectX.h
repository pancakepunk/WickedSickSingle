#pragma once

#include "GraphicsAPI.h"

#include "D3D/DXIncludes.h"
#include "Model/DxModel.h"
#include "Shader/DxShader.h"
#include "Texture/DxTexture.h"
#include "RenderTarget/DxRenderTarget.h"
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
    Texture* MakeTexture(const TextureDesc& desc);
    Texture* MakeTexture(const std::string& name);
    Texture* MakeTexture(const std::vector<unsigned char>& tex,
                         const TextureDesc& desc);
    Shader* MakeShader(const std::string& name, Shader::ShaderCallback callback);

    RenderTarget* MakeRenderTarget(const RenderTargetDesc& desc);

    void BeginScene();
    void EndScene();
    
    SwapChain* GetSwapChain();

    void SetBlendType(BlendType::Enum type);

    void SetRenderTargets() override;

    void ClearShaderResources();

    void FlushDepth() override;

    void SetDepthType(DepthType::Enum type);
    //void SetRenderTarget(Texture*)
    
    
  private:
    
    

    void clear_buffers();
 
    SwapChain*                swap_chain_;
    Factory<DxModel>          model_factory_;
    Factory<DxShader>         shader_factory_;
    Factory<DxTexture>        texture_factory_;
    Factory<DxRenderTarget>   render_target_factory_;

    DxRenderTarget* back_buffer_target_;
    //grody dx stuff
    //ID3D11RenderTargetView* back_buffer_;
    //ID3D11DepthStencilState* depth_stencil_state_;
    DepthType::Enum depth_type_;
    ID3D11DepthStencilState* depth_stencil_states_[DepthType::Count];

    ID3D11Texture2D* depth_stencil_buffer_;
    ID3D11DepthStencilView* depth_stencil_view_;
    //ID3D11RenderTargetView* back_buffer_view_;

    ID3D11RasterizerState* rasterizer_state_;

    BlendType::Enum blend_type_;

    ID3D11BlendState* blend_states_[BlendType::Count];

    ID3D11BlendState* additive_blend_;
    ID3D11BlendState* normal_blend_;

    
  };
}