#pragma once
#include "Constraint.h"

namespace WickedSick
{
  class RigidBody;

  class PointConstraint : public Constraint
  {
    public:
      PointConstraint(RigidBody* first, RigidBody* second);
    private:
      void solve_constraint();
      RigidBody* first_;
      RigidBody* second_;
      Vector3 constraint_point_;
  };
}

