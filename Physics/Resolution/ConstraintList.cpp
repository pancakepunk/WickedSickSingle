#include "Precompiled.h"

#include "PhysicsPrecompiled.h"
#include "ConstraintList.h"
#include "Constraint.h"

namespace WickedSick
{
  ConstraintList::ConstraintList()
  {
  }
  
  ConstraintList::~ConstraintList()
  {
  }
  
  std::vector<ConstraintList::SP_Constraint>& ConstraintList::Get()
  { 
    return constraints_; 
  }
  
  void ConstraintList::push(SP_Constraint constraint)
  { 
    constraints_.push_back(constraint); 
  }
  
  ConstraintList::SP_Constraint ConstraintList::front()
  { 
    return constraints_.front(); 
  }
  
  ConstraintList::SP_Constraint ConstraintList::back()
  { 
    return constraints_.back(); 
  }

  void ConstraintList::Update(const float& dt)
  {
    for(unsigned i = 0; i < constraints_.size(); ++i)
    {

      if(!constraints_[i]->IsSatisfied())
      {
        constraints_[i]->Iterate(dt);
      }

      // this code is never run? //need to work on this sometime....
      //else
      //{
      //  vector_remove(constraints_, i);
      //  if(!constraints_.size())
      //  {
      //    break;
      //  }
      //  //we need to iterate the current one as well
      //}
    }
  }
}
