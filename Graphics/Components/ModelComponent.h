#pragma once

#include "Core/CoreInterface.h"
#include "Model/Vertex.h"
#include "meta/MetaInterface.h"

#include "General/GraphicsTypes.h"
#include "Graphics/Texture/Texture.h"

namespace WickedSick
{
  struct Material
  {
    MetaDef;
    Material();
    Vector4 emissiveColor;
    Vector4 ambientColor;
    Vector4 diffuseColor;
    Vector4 specularColor;

    float emissiveConstant;
    float ambientConstant;
    float diffuseConstant;
    float specularConstant;

    float shininess;
  };
  class Model;


  class ModelComponent : public Component
  {
  public:

    MetaDef;
    ModelComponent(const std::string& model = "",
                   GameObject* owner = nullptr);
    ~ModelComponent();
    void Destroy();
    void SetModel(const std::string& model);
    void Initialize() override;
    void SetShader(const std::string& shader);
    void Update(float dt) override;
    void Clone(Component* source) override;

    Model* GetBase();
    std::string GetShader();
    void SetDrawType(DrawType::Enum type);
    DrawType::Enum GetDrawType();
    std::vector<Vertex> GetDebugVerts();
    Material& GetMaterial();

    void SetTexture(const std::string& newTex);
    void SetNormalMap(const std::string& newTex);
    
    std::string GetTexture();
    std::string GetNormalMap();
  private:

    Material material_props_;
    DrawType::Enum draw_type_;
    std::string model_;
    std::string shader_;
    std::string texture_;
    std::string normal_map_;
    Model* base_;

    //std::set<Texture*> textures_;

  };
}
