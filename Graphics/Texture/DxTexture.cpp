#include "Precompiled.h"
//#include "MasterPrecompiled.h" //really need this
#include "DxTexture.h"
#include "Graphics/GraphicsInterface.h"
#include "Graphics/System/Graphics.h"
#include "Core/CoreInterface.h"

#include "Graphics/D3D/DXIncludes.h"
#include "Graphics/API/DirectX.h"
#include "Graphics/D3D/SwapChain.h"
#include "Graphics/D3D/Device.h"


namespace WickedSick
{
  DxTexture::DxTexture(const TextureDesc& desc)
  : Texture(desc),
    texture_view_(nullptr),
    texture_2d_(nullptr)
  {
  }

  DxTexture::DxTexture(const std::string& texName)
  : texture_view_(nullptr),
    texture_2d_(nullptr),
    Texture(texName)
  {
  }

  DxTexture::DxTexture(const std::vector<unsigned char>& tex, 
                       const TextureDesc& desc)
  : texture_view_(nullptr),
    texture_2d_(nullptr),
    Texture(tex, desc)
  {
    
  }

  DxTexture::~DxTexture()
  {
    Cleanup();
  }

  void DxTexture::Initialize()
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    DirectX* dx = (DirectX*)gSys->graphicsAPI;
    if(tex_.empty())
    {
      std::wstring wStrBecauseWhyNot(source_.begin(), source_.end());
      ID3D11Resource* pleasegivemethesize;
      DxError(::DirectX::CreateWICTextureFromFile(dx->GetSwapChain()->device->D3DDevice,
                                                  wStrBecauseWhyNot.c_str(),
                                                  &pleasegivemethesize,
                                                  &texture_view_));
      ID3D11Texture2D* getthesize;
      pleasegivemethesize->QueryInterface<ID3D11Texture2D>(&getthesize);
      D3D11_TEXTURE2D_DESC desc;
      getthesize->GetDesc(&desc);
      desc_.size.x = desc.Width;
      desc_.size.y = desc.Height;
      
    }
    else
    {
      D3D11_TEXTURE2D_DESC desc;
      ZeroMemory(&desc, sizeof(D3D11_TEXTURE2D_DESC));
      desc.ArraySize = 1;
      desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
      if(desc_.target)
      {
        desc.BindFlags |= D3D11_BIND_RENDER_TARGET;
      }
      desc.CPUAccessFlags = 0;
      desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
      desc.Width = desc_.size.x;
      desc.Height = desc_.size.y;
      desc.MipLevels = 0;
      desc.MiscFlags = 0;
      desc.SampleDesc.Count = 1;
      desc.SampleDesc.Quality = 0;
      if(desc_.writable)
      {
        desc.Usage = D3D11_USAGE_STAGING;
      }
      else
      {
        desc.Usage = D3D11_USAGE_DEFAULT;
      }
      
      auto& device = dx->GetSwapChain()->device->D3DDevice;
      DxError(device->CreateTexture2D(&desc,
                                      NULL,
                                      &texture_2d_));
      if(!desc_.target)
      {
        dx->GetSwapChain()->device->D3DContext->UpdateSubresource(texture_2d_,
                                                                  0,
                                                                  nullptr,
                                                                  &tex_[0],
                                                                  512 * 4,
                                                                  512 * 512 * 4);
      }
      DxError(device->CreateShaderResourceView(texture_2d_,
                                               nullptr,
                                               &texture_view_));

    }
   
  }

  void DxTexture::Cleanup()
  {
    texture_view_->Release();
  }

  void* DxTexture::GetResourcePointer()
  {
    return (void*)texture_view_;
  }
  void * DxTexture::GetTexturePointer()
  {
    return texture_2d_;
  }
}


