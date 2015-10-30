#pragma once

namespace WickedSick
{
  enum PhysicsMaterialType
  {
    PMT_UNIT,
    PMT_MAP,
    PMT_PHYSICSOBJECT,
    PMT_SUPER_BOUNCY,
    PMT_COUNT
  };

  struct PhysicsMaterial
  {
    PhysicsMaterial(const std::string& name,
                    float dynamicFriction = 0.0f, 
                    float staticFriction = 0.0f, 
                    float restitution = 1.0f, 
                    float density = 1.0f);
    std::string Name;
    float DynamicFriction;
    float StaticFriction;
    float Restitution;
    float Density;
    
  };
}
