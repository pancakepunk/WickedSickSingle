#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "DxShader.h"
#include "D3D/DXIncludes.h"
#include "API/DirectX.h"
#include "System/Graphics.h"
#include "D3D/SwapChain.h"
#include "D3D/Device.h"
#include "General/GraphicsUtility.h"
#include "Buffer/DxBuffer.h"

#include "Graphics/Texture/DxTexture.h"

#include <filesystem>

namespace WickedSick
{

  DxShader::DxShader(const std::string& name,
                     ShaderCallback callback) 
  : vertex_shader_(nullptr),
    pixel_shader_(nullptr),
    compute_shader_(nullptr),
    geometry_shader_(nullptr),
    layout_(nullptr),
    Shader(name, callback)
  {

  }
  
  DxShader::~DxShader()
  {

  }

  void DxShader::Initialize()
  {

    if (!Compile(true))
    {
      ConsolePrint("Shader " + name_ + " failed to compile.\n");
    }
    else
    {
      ReflectBuffers();
      
    }
    D3D11_SAMPLER_DESC samplerDesc;

    // Create a texture sampler state description.
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.MipLODBias = 0.0f;
    samplerDesc.MaxAnisotropy = 1;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.BorderColor[0] = 0;
    samplerDesc.BorderColor[1] = 0;
    samplerDesc.BorderColor[2] = 0;
    samplerDesc.BorderColor[3] = 0;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

    // Create the texture sampler state.
    DirectX* dx = reinterpret_cast<DirectX*>(Graphics::graphicsAPI);
    SwapChain* swapChain = dx->GetSwapChain();
    DxError(swapChain->device->D3DDevice->CreateSamplerState(&samplerDesc, &tex_sampler_state_));
  }

  void DxShader::SetParameters(const std::vector<ParamPasser>& params)
  {
    if(!pixel_shader_ || !vertex_shader_)
    {
      return;
    }
    DirectX* dx = reinterpret_cast<DirectX*>(Graphics::graphicsAPI);
    SwapChain* swapChain = dx->GetSwapChain();
    ID3D11DeviceContext* context = swapChain->device->D3DContext;


    for(auto& it : params)
    {
      auto& bufIt = constant_buffers_.find(it.Buffer);
      if(bufIt != constant_buffers_.end())
      {
        DxBuffer* buf = (DxBuffer*)bufIt->second;
        buf->SetParameter(it.Name, (void*)it.Data);
      }
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    DxBuffer* buffer;
    //std::vector<ID3D11Buffer*> bufferList;
    ID3D11Buffer* curBuf = nullptr;
    for(auto cbIt : constant_buffers_)
    {
      buffer = (DxBuffer*) cbIt.second;
      curBuf = (ID3D11Buffer*) buffer->BufferPointer();
      DxError(context->Map(curBuf,
                           0,
                           D3D11_MAP_WRITE_DISCARD,
                           0,
                           &mappedResource));

      memcpy(mappedResource.pData, buffer->GetData(), buffer->GetSize());
      context->Unmap(curBuf, 0);

      
      //bufferList.push_back((ID3D11Buffer*)buffer->BufferPointer());
      size_t vertIndex = buffer->GetIndex(ShaderType::Vertex);
      size_t pixIndex = buffer->GetIndex(ShaderType::Pixel);
      if(vertIndex != DxBuffer::invalidIndex)
      {
        context->VSSetConstantBuffers(vertIndex, 1, &curBuf);
      }
      if(pixIndex != DxBuffer::invalidIndex)
      {
        context->PSSetConstantBuffers(pixIndex, 1, &curBuf);
      }
    }

    ID3D11ShaderResourceView* texView;
    for(size_t i = 0; i < textures_.size(); ++i)
    {
      if(!textures_[i])
      {
        __debugbreak();
      }
      texView = (ID3D11ShaderResourceView*)textures_[i]->GetResourcePointer();
      context->PSSetShaderResources(i, 1, &texView);
    }

    
  }

  bool DxShader::Compile(bool forceCompile)
  {
    DirectX* dx = reinterpret_cast<DirectX*>(Graphics::graphicsAPI);
    SwapChain* swapChain = dx->GetSwapChain();
    ID3D11Device* device = swapChain->device->D3DDevice;

    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* pixelShaderBuffer = nullptr;
    
    std::string baseFileName = files_[ShaderType::Vertex].Path();
    std::wstring fileStr(baseFileName.begin(), baseFileName.end());
    if (files_[ShaderType::Vertex].WasModified() || forceCompile)
    {
      while (!files_[ShaderType::Vertex].Writable())
      {
        Sleep(1);
      }
      DxError(D3DCompileFromFile (fileStr.c_str(),
                                  NULL, 
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE, 
                                  "main",
                                  "vs_5_0",
                                #ifdef _DEBUG
                                  D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL, 
                                #else
                                  D3D10_SHADER_ENABLE_STRICTNESS,
                                #endif
                                  0, 
                                  &vertexShaderBuffer, 
                                  &errorMessage));
      if (errorMessage)
      {
        //error?
        if (vertexShaderBuffer)
        {
          vertexShaderBuffer->Release();
        }
        ConsolePrint("Shader Error: ");
        ConsolePrint((char*)errorMessage->GetBufferPointer());
        std::string filename(fileStr.begin(), fileStr.end());
        ConsolePrint("\n in shader " + filename + "\n");
        errorMessage->Release();
        return false;
      }
      if (vertex_shader_)
      {
        vertex_shader_->Release();
      }
      DxError(device->CreateVertexShader( vertexShaderBuffer->GetBufferPointer(), 
                                          vertexShaderBuffer->GetBufferSize(), 
                                          NULL, 
                                          &vertex_shader_));
      InitLayout(vertexShaderBuffer);
      vertexShaderBuffer->Release();
      ConsolePrint("Vertex shader " + name_ + " compiled successfully.\n");
    }
    
    if (files_[ShaderType::Pixel].WasModified() || forceCompile)
    {
      while (!files_[ShaderType::Pixel].Writable())
      {
        Sleep(1);
      }
      baseFileName = files_[ShaderType::Pixel].Path();
      fileStr = std::wstring(baseFileName.begin(), baseFileName.end());
      DxError(D3DCompileFromFile (fileStr.c_str(),
                                  NULL, 
                                  D3D_COMPILE_STANDARD_FILE_INCLUDE, 
                                  "main", 
                                  "ps_5_0",
                                #ifdef _DEBUG
                                  D3D10_SHADER_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PREFER_FLOW_CONTROL,
                                #else
                                  D3D10_SHADER_ENABLE_STRICTNESS,
                                #endif
                                  0, 
                                  &pixelShaderBuffer, 
                                  &errorMessage));
      if (errorMessage)
      {
        if (pixelShaderBuffer)
        {
          pixelShaderBuffer->Release();
        }
        ConsolePrint("Shader Error: ");
        ConsolePrint((char*)errorMessage->GetBufferPointer());
        std::string filename(fileStr.begin(), fileStr.end());
        ConsolePrint("\n in shader " + filename + "\n");
        errorMessage->Release();
        return false;
      }
      if (pixel_shader_)
      {
        pixel_shader_->Release();
      }
      DxError(device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(),  
                                        pixelShaderBuffer->GetBufferSize(),   
                                        NULL, 
                                        &pixel_shader_));
      ReflectPixelShader(pixelShaderBuffer);
      pixelShaderBuffer->Release();
      ConsolePrint("Pixel shader " + name_ + " compiled successfully.\n");
    }
    
    return true;
  }

  void DxShader::ReflectBuffers()
  {

  }

  //DXGI_FORMAT find_param_type(const ShaderParam& param)
  //{
  //  switch (param.inputType)
  //  {
  //    case DataType::Int:
  //    {
  //      switch (param.size)
  //      {
  //        case sizeof(int) * 4:
  //          return DXGI_FORMAT_R32G32B32A32_SINT;
  //        case sizeof(int) * 3:
  //          return DXGI_FORMAT_R32G32B32_SINT;
  //        case sizeof(int) * 2:
  //          return DXGI_FORMAT_R32G32_SINT;
  //        case sizeof(int):
  //          return DXGI_FORMAT_R32_SINT;
  //        default:
  //          break;
  //      }
  //    }
  //    case DataType::Float:
  //    {
  //      switch (param.size)
  //      {
  //        case sizeof(float) * 4:
  //          return DXGI_FORMAT_R32G32B32A32_FLOAT;
  //        case sizeof(float) * 3:
  //          return DXGI_FORMAT_R32G32B32_FLOAT;
  //        case sizeof(float) * 2:
  //          return DXGI_FORMAT_R32G32_FLOAT;
  //        case sizeof(float) :
  //          return DXGI_FORMAT_R32_FLOAT;
  //        default:
  //          break;
  //      }
  //    }
  //    case DataType::Unsigned:
  //    {
  //      switch (param.size)
  //      {
  //        case sizeof(unsigned) * 4:
  //          return DXGI_FORMAT_R32G32B32A32_UINT;
  //        case sizeof(unsigned) * 3:
  //          return DXGI_FORMAT_R32G32B32_UINT;
  //        case sizeof(unsigned) * 2:
  //          return DXGI_FORMAT_R32G32_UINT;
  //        case sizeof(unsigned) :
  //          return DXGI_FORMAT_R32_UINT;
  //        default:
  //          break;
  //      }
  //    }
  //    case DataType::Typeless:
  //    {
  //      switch (param.size)
  //      {
  //        case sizeof(unsigned) * 4:
  //          return DXGI_FORMAT_R32G32B32A32_TYPELESS;
  //        case sizeof(unsigned) * 3:
  //          return DXGI_FORMAT_R32G32B32_TYPELESS;
  //        case sizeof(unsigned) * 2:
  //          return DXGI_FORMAT_R32G32_TYPELESS;
  //        case sizeof(unsigned) :
  //          return DXGI_FORMAT_R32_TYPELESS;
  //        default:
  //          break;
  //      }
  //    }
  //    default:
  //      break;
  //  }
  //  return DXGI_FORMAT_UNKNOWN;
  //}

  bool DxShader::InitLayout(ID3D10Blob* vertexShaderBuffer)
  {

    DirectX* dx = reinterpret_cast<DirectX*>(Graphics::graphicsAPI);
    SwapChain* swapChain = dx->GetSwapChain();
    ID3D11Device* device = swapChain->device->D3DDevice;


    //for now. later we might have to change this.
    if(layout_)
    {
      layout_->Release();
      layout_ = nullptr;
    }
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc = ReflectVertexShader(vertexShaderBuffer);

    int bufSize = vertexShaderBuffer->GetBufferSize();
    DxError(device->CreateInputLayout(&inputLayoutDesc[0],
                                      inputLayoutDesc.size(),
                                      vertexShaderBuffer->GetBufferPointer(),
                                      vertexShaderBuffer->GetBufferSize(),
                                      &layout_));

    //vertShaderReflection->Release();
    return true;
  }

  std::vector<D3D11_INPUT_ELEMENT_DESC> DxShader::ReflectVertexShader(ID3D10Blob* vertexShaderBuffer)
  {
    ID3D11ShaderReflection* vertShaderReflection = nullptr;
    DxError(D3DReflect(vertexShaderBuffer->GetBufferPointer(),
                       vertexShaderBuffer->GetBufferSize(),
                       IID_ID3D11ShaderReflection,
                       (void**) &vertShaderReflection));

    D3D11_SHADER_DESC shaderDesc;
    vertShaderReflection->GetDesc(&shaderDesc);

    // Read input layout description from shader info
    std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
    for(unsigned i = 0; i < shaderDesc.InputParameters; i++)
    {
      ParamTypeInfo newParam;
      D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
      vertShaderReflection->GetInputParameterDesc(i, &paramDesc);

      // fill out input element desc
      D3D11_INPUT_ELEMENT_DESC elementDesc;
      elementDesc.SemanticName = paramDesc.SemanticName;
      elementDesc.SemanticIndex = paramDesc.SemanticIndex;
      elementDesc.InputSlot = 0;
      elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
      elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
      elementDesc.InstanceDataStepRate = 0;
      if(paramDesc.Mask == 1)
      {
        if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
      }
      else if(paramDesc.Mask <= 3)
      {
        if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
      }
      else if(paramDesc.Mask <= 7)
      {
        if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
      }
      else if(paramDesc.Mask <= 15)
      {
        if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
        else if(paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
      }
      inputLayoutDesc.push_back(elementDesc);
    }

    for(size_t i = 0; i < shaderDesc.ConstantBuffers; i++)
    {

      ID3D11ShaderReflectionConstantBuffer* pConstantReflection = vertShaderReflection->GetConstantBufferByIndex(i);


      D3D11_SHADER_BUFFER_DESC desc;

      DxError(pConstantReflection->GetDesc(&desc));
      DxBuffer* newBuf = new DxBuffer(desc.Name,
                                      0,
                                      0,
                                      nullptr,
                                      desc.Size,
                                      BufferType::Constant,
                                      AccessType::CpuWrite,
                                      UsageType::Dynamic);

      for(size_t varInd = 0; varInd < desc.Variables; ++varInd)
      {
        ID3D11ShaderReflectionVariable* curVar = pConstantReflection->GetVariableByIndex(varInd);
        D3D11_SHADER_VARIABLE_DESC varDesc;
        DxError(curVar->GetDesc(&varDesc));
        newBuf->AddParameter({varDesc.Name, varDesc.Size, varDesc.StartOffset});

      }

      newBuf->Initialize();
      AddConstantBuffer(newBuf);
      newBuf->SetIndex(ShaderType::Vertex, i);
      //m_cBuffer[i] = new d3d::ConstantBuffer(*m_pDevice, desc.Size);
      
    }
    return inputLayoutDesc;
  }

  void DxShader::ReflectPixelShader(ID3D10Blob* pixelShaderBuffer)
  {

    ID3D11ShaderReflection* pixelShaderReflection = nullptr;
    DxError(D3DReflect(pixelShaderBuffer->GetBufferPointer(),
                       pixelShaderBuffer->GetBufferSize(),
                       IID_ID3D11ShaderReflection,
                       (void**) &pixelShaderReflection));

    D3D11_SHADER_DESC shaderDesc;
    pixelShaderReflection->GetDesc(&shaderDesc);

    for(size_t i = 0; i < shaderDesc.ConstantBuffers; i++)
    {

      ID3D11ShaderReflectionConstantBuffer* pConstantReflection = pixelShaderReflection->GetConstantBufferByIndex(i);


      D3D11_SHADER_BUFFER_DESC desc;

      DxError(pConstantReflection->GetDesc(&desc));
      DxBuffer* newBuf = new DxBuffer(desc.Name,
                                      0,
                                      0,
                                      nullptr,
                                      desc.Size,
                                      BufferType::Constant,
                                      AccessType::CpuWrite,
                                      UsageType::Dynamic);

      for(size_t varInd = 0; varInd < desc.Variables; ++varInd)
      {
        ID3D11ShaderReflectionVariable* curVar = pConstantReflection->GetVariableByIndex(varInd);
        D3D11_SHADER_VARIABLE_DESC varDesc;
        DxError(curVar->GetDesc(&varDesc));
        newBuf->AddParameter({varDesc.Name, varDesc.Size, varDesc.StartOffset});

      }

      newBuf->Initialize();
      AddConstantBuffer(newBuf);
      newBuf->SetIndex(ShaderType::Pixel, i);
      //m_cBuffer[i] = new d3d::ConstantBuffer(*m_pDevice, desc.Size);

    }
  }

  void DxShader::Render(int count)
  {
    if(!pixel_shader_ || !vertex_shader_)
    {
      return;
    }
    DirectX* dx = reinterpret_cast<DirectX*>(Graphics::graphicsAPI);
    SwapChain* swapChain = dx->GetSwapChain();
    ID3D11DeviceContext* context = swapChain->device->D3DContext;
    context->IASetInputLayout(layout_);
    context->PSSetSamplers(0, 1, &tex_sampler_state_);
    context->VSSetShader(vertex_shader_, NULL, 0);
    context->PSSetShader(pixel_shader_, NULL, 0);

    context->Draw(count, 0);

    
  }

  void DxShader::SetShaderDir(const std::string& dir)
  {
    ConsolePrint("Why do you call");
  }


}