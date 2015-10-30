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
  DxTexture::DxTexture(const std::string & texName) 
  : texture_view_(nullptr),
    Texture(texName)
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
    std::wstring wStrBecauseWhyNot(source_.begin(), source_.end());
    DxError(::DirectX::CreateWICTextureFromFile(dx->GetSwapChain()->device->D3DDevice,
                                                wStrBecauseWhyNot.c_str(),
                                                nullptr,
                                                &texture_view_));
    //DXError()
  }

  void DxTexture::Cleanup()
  {
    texture_view_->Release();
  }

  void* DxTexture::GetTexturePointer()
  {
    return (void*)texture_view_;
  }
}


