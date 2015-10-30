#pragma once

#include "Constraint.h"
namespace WickedSick
{
  class Constraint;
  class ConstraintList
  {
    public:
      typedef std::shared_ptr<Constraint> SP_Constraint;

    ConstraintList();
    ~ConstraintList();
    std::vector<SP_Constraint>& Get();
    void push(SP_Constraint constraint);
    void remove(SP_Constraint constraint);
    SP_Constraint front();
    SP_Constraint back();
    void Update(const float& dt);
    private:
      std::vector<SP_Constraint> constraints_;
  };
}
