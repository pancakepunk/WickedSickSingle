
#include "PhysicsPrecompiled.h"
#include "ContactEdge.h"
#include "Architecture/Collider.h"
namespace WickedSick
{
  ContactEdge::ContactEdge(Collider* self, Collider* other) : self_(self),
                                                              other_(other)
  {
  }

  ContactEdge::~ContactEdge()
  {
  }

}
