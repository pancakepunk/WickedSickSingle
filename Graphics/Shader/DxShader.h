#pragma once

#include "D3D/DXIncludes.h"
#include "Shader/Shader.h"

struct ID3D11VertexShader;
struct ID3D11PixelShader;
struct ID3D11ComputeShader;
struct ID3D11GeometryShader;
struct ID3D11InputLayout;
struct ID3D10Blob;

namespace WickedSick
{
  struct LightBuffer
  {
    Matrix4 modelToWorld;
    Matrix4 worldToClip;
    Vector3 lightDir;
    Vector3 cameraVec;
  };

  class DxShader : public Shader
  {
  public:
    DxShader(const std::string& name,
             ShaderCallback callback);
    ~DxShader();

    void Initialize();

    bool Compile(bool forceCompile = false);
    void ReflectBuffers();

    bool InitLayout(ID3D10Blob* vertexShaderBuffer);

    std::vector<D3D11_INPUT_ELEMENT_DESC> ReflectVertexShader(ID3D10Blob * vertexShaderBuffer);

    void ReflectPixelShader(ID3D10Blob * pixelShaderBuffer);

    void Render(int count) override;

    void SetShaderDir(const std::string& dir);
    

    void SetParameters(const std::vector<ParamPasser>& params);


  private:



    ID3D11VertexShader*   vertex_shader_;
    ID3D11PixelShader*    pixel_shader_;
    ID3D11ComputeShader*  compute_shader_;
    ID3D11GeometryShader* geometry_shader_;
    ID3D11InputLayout*    layout_;

    ID3D11SamplerState*   tex_sampler_state_;

  };
}
