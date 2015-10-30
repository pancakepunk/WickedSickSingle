#pragma once
namespace WickedSick
{
  class Constraint
  {
    public:
      Constraint();
      ~Constraint();
      virtual void Iterate(const float& dt) = 0;
      virtual void Initialize() = 0;
      bool IsSatisfied(){return satisfied_;}
      void SetSatisfied(bool val){satisfied_ = val;}

    protected:
      bool satisfied_;
  };
  //base Constraint class
  //we'll inherit from it to create different kinds of constraints
}

