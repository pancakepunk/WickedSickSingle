
#include "PhysicsPrecompiled.h"
#include "Constraint.h"
namespace WickedSick
{
  Constraint::Constraint()
    : satisfied_(false)
  {
  }

  Constraint::~Constraint()
  {
    if(!satisfied_)
    {
      std::cout << "base" << std::endl;
    }
  }

}