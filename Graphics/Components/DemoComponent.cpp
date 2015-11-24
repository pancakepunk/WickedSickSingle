#include "Precompiled.h"
#include "DemoComponent.h"
#include "Graphics/GraphicsInterface.h"

namespace WickedSick
{
  DemoComponent::DemoComponent() 
  : demo_type_(DemoType::None),
    mapping_type_(MappingType::Planar),
    entity_type_(EntityType::Centroid),
    Component(CT_DemoComponent)
  {
  }

  DemoComponent::~DemoComponent()
  {
  }

  void DemoComponent::Initialize()
  {
  }

  void DemoComponent::Update(float dt)
  {
    ModelComponent* comp = (ModelComponent*)GetSibling(CT_ModelComponent);
    switch(demo_type_)
    {
      case DemoType::TextureMapping:
      {
        if(comp)
        {
          std::string shaderName;
          switch(entity_type_)
          {
            case EntityType::Centroid:
              shaderName.append("centroid");
              break;
            case EntityType::Normal:
              shaderName.append("normal");
              break;
            case EntityType::Reflection:
              shaderName.append("reflection");
              break;
            case EntityType::Position:
              shaderName.append("position");
              break;
          }

          switch(mapping_type_)
          {
            case MappingType::Spherical:
              shaderName.append("spherical");
              break;
            case MappingType::Cylindrical:
              shaderName.append("cylindrical");
              break;
            case MappingType::Cubic:
              shaderName.append("cubic");
              break;
            case MappingType::Planar:
              shaderName.append("planar");
              break;
          }

          comp->SetShader(shaderName);
        }
        break;
      }
      case DemoType::NormalMapping:
      {
        comp->SetShader("normalmapping");
        break;
      }

    }
  }

  EntityType::Enum DemoComponent::GetEntityType()
  {
    return entity_type_;
  }

  MappingType::Enum DemoComponent::GetMappingType()
  {
    return mapping_type_;
  }

  DemoType::Enum DemoComponent::GetDemoType()
  {
    return demo_type_;
  }

  void DemoComponent::SetEntityType(EntityType::Enum type)
  {
    entity_type_ = type;
  }

  void DemoComponent::SetMappingType(MappingType::Enum type)
  {
    mapping_type_ = type;
  }

  void DemoComponent::SetDemoType(DemoType::Enum type)
  {
    demo_type_ = type;
  }
}


RegisterEnum(WickedSick, EntityType, Enum, Count)
}
RegisterEnum(WickedSick, MappingType, Enum, Count)
}
RegisterEnum(WickedSick, DemoType, Enum, Count)
}


RegisterType(WickedSick, DemoComponent)
RegisterMember(entity_type_);
RegisterMember(mapping_type_);
RegisterMember(demo_type_);
}
