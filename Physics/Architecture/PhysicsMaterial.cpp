
#include "PhysicsPrecompiled.h"
#include "PhysicsMaterial.h"

namespace WickedSick
{
  PhysicsMaterial::PhysicsMaterial( const std::string& name,
                                    float dynamicFriction, 
                                    float staticFriction, 
                                    float restitution, 
                                    float density) 
                                    : Name(name),
                                      DynamicFriction(dynamicFriction),
                                      StaticFriction(staticFriction),
                                      Restitution(restitution),
                                      Density(density)                           
  {
  }
}
