#pragma once

#include "Core/CoreInterface.h"

#include "Graphics/Model/Model.h"

namespace WickedSick
{
  namespace EntityType
  {
    enum Enum
    {
      Centroid,
      Normal,
      Position,
      Reflection,
      Count
    };
  };

  //namespace MappingType
  //{
  //  enum Enum
  //  {
  //    Cylindrical,
  //    Spherical,
  //    Planar,
  //    Cubic,
  //    Count
  //  };
  //};

  namespace DemoType
  {
    enum Enum
    {
      None,
      NormalMapping,
      TextureMapping,
      Count
    };
  }


  class DemoComponent : public Component
  {
    public:
      MetaDef;
      DemoComponent();
      ~DemoComponent(); 
      void Initialize();
      void Update(float dt);
      
      EntityType::Enum GetEntityType();
      MappingType::Enum GetMappingType();
      DemoType::Enum GetDemoType();

      void SetEntityType(EntityType::Enum type);
      void SetMappingType(MappingType::Enum type);
      void SetDemoType(DemoType::Enum type);
    private:

      DemoType::Enum demo_type_;

      EntityType::Enum entity_type_;
      MappingType::Enum mapping_type_;

  };
}
