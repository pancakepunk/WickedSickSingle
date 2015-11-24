#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "ModelComponent.h"

#include "Model/Model.h"
#include "Core/CoreInterface.h"
#include "System/Graphics.h"
#include "General/GraphicsTypes.h"
#include "Input/InputInterface.h"
#include "Physics/PhysicsInterface.h"

namespace WickedSick
{
  ModelComponent::ModelComponent(const std::string& model,
                                 GameObject* owner)
  : name_("ModelComponent"),
    model_(model),
    base_(nullptr),
    draw_type_(DrawType::Default),
    Component(CT_ModelComponent, owner)
  {
    
  }

  ModelComponent::~ModelComponent()
  {

  }

  void ModelComponent::Initialize()
  {
    Engine* core = Engine::GetCore();
    Graphics* gSys = (Graphics*)core->GetSystem(ST_Graphics);
    base_ = gSys->GetModel(model_);
    if (base_)
    {
      base_->AddInstance(this);
    }
  }

  void ModelComponent::Update(float dt)
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    gSys->AddDebugModel(this);

    Input* input = (Input*) Engine::GetCore()->GetSystem(ST_Input);
    InputHandler* handler = input->GetHandler();
    Transform* tr = (Transform*)GetSibling(CT_Transform);
    PhysicsComponent* phys = (PhysicsComponent*) GetSibling(CT_PhysicsComponent);
    if(handler->Check("MoveRight"))
    {
      tr->SetPosition(tr->GetPosition() + Vector3(1.0f,0.0f,0.0f) * dt);
      phys->Reset();
    }
    Model* newModel = gSys->GetModel(model_);
    if(newModel && base_ != newModel)
    {
      base_->RemoveInstance(this);
      base_ = newModel;
      base_->AddInstance(this);
    }


  }

  void ModelComponent::Destroy()
  {
    base_->RemoveInstance(this);
  }

  void ModelComponent::SetModel(const std::string & model)
  {
    model_ = model;
  }

  std::vector<Vertex> ModelComponent::GetDebugVerts()
  {

    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    Matrix4 modelToWorld;
    Camera* camera = gSys->GetCamera();
    Transform* tr = (Transform*) GetSibling(CT_Transform);

    std::vector<Vertex> endVerts;
    switch(draw_type_)
    {
      case DrawType::VertexNormals:
      {
        std::vector<Vertex> verts = base_->GetVerts();
        Vertex newVert;
        endVerts.reserve(verts.size() * 2);
        for(auto it = verts.begin(); it != verts.end(); ++it)
        {
          endVerts.push_back(*it);
          newVert.normal = it->normal;
          newVert.position = it->position;
          newVert.position.x += (1.0f / tr->GetScale().x) * newVert.normal.x * 0.5f;
          newVert.position.y += (1.0f / tr->GetScale().y) * newVert.normal.y * 0.5f;
          newVert.position.z += (1.0f / tr->GetScale().z) * newVert.normal.z * 0.5f;
          endVerts.push_back(newVert);
          //it = verts.insert(it, newVert);
        }

        break;
      }
      case DrawType::FaceNormals:
      {
        std::vector<Vertex> verts = base_->GetVerts();
        Vertex newVert;
        //std::vector<Face> faces = base_->GetFaces();
        //endVerts.reserve(faces.size() * 2);
        //Vector3 v0v1;
        //Vector3 v0v2;
        //for(auto it = faces.begin(); it != faces.end(); ++it)
        //{
        //  newVert.position =  (verts[it->indices[0]].position + 
        //                       verts[it->indices[1]].position +
        //                       verts[it->indices[2]].position) * (1.0f / 3.0f);
        //  
        //
        //  v0v1 = (verts[it->indices[1]].position - verts[it->indices[0]].position).GetNormalized();
        //  v0v2 = (verts[it->indices[2]].position - verts[it->indices[0]].position).GetNormalized();
        //  newVert.normal =  v0v1.Cross(v0v2);
        //  endVerts.push_back(newVert);
        //  newVert.position.x += (1.0f/tr->GetScale().x) * newVert.normal.x * 0.5f;
        //  newVert.position.y += (1.0f/tr->GetScale().y) * newVert.normal.y * 0.5f;
        //  newVert.position.z += (1.0f/tr->GetScale().z) * newVert.normal.z * 0.5f;
        //  endVerts.push_back(newVert);
        //}

        
        break;
      }
      case DrawType::Wireframe:
      {
        break;
      }
      case DrawType::Default:
      default:
        return std::vector<Vertex>();
        break;
    }
    return endVerts;
  }

  Material& ModelComponent::GetMaterial()
  {
    return material_props_;
  }

  void ModelComponent::SetTexture(const std::string& texture)
  {
    texture_ = texture;
  }

  void ModelComponent::SetNormalMap(const std::string & newTex)
  {
    normal_map_ = newTex;
  }

  Model* ModelComponent::GetBase()
  {
    return base_;
  }

  void ModelComponent::SetShader(const std::string& shader)
  {
    shader_ = shader;
  }

  std::string ModelComponent::GetShader()
  {
    return shader_;
  }

  void ModelComponent::SetDrawType(DrawType::Enum type)
  {
    draw_type_ = type;
  }

  DrawType::Enum ModelComponent::GetDrawType()
  {
    return draw_type_;
  }

  std::string ModelComponent::GetTexture()
  {
    return texture_;
  }

  std::string ModelComponent::GetNormalMap()
  {
    return normal_map_;
  }


  Material::Material() 
  : emissiveConstant(0.0f),
    ambientConstant(0.0f),
    diffuseConstant(0.0f),
    specularConstant(0.0f),
    shininess(0.0f)
  {
  }
}



RegisterType(WickedSick, Material)
RegisterMember(emissiveColor);
RegisterMember(ambientColor);
RegisterMember(diffuseColor);
RegisterMember(specularColor);

RegisterMember(emissiveConstant);
RegisterMember(ambientConstant);
RegisterMember(diffuseConstant);
RegisterMember(specularConstant);

RegisterMember(shininess);
}


RegisterType(WickedSick, ModelComponent)
RegisterMember(material_props_);
RegisterMember(draw_type_);
RegisterMember(shader_);
RegisterMember(model_);
RegisterMember(texture_);
RegisterMember(normal_map_);
}