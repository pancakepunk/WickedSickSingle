
#include "PhysicsPrecompiled.h"
#include "ContactManifold.h"
#include "Contact.h"

#include "ContactConstraint.h"
#include "Components\PhysicsComponent.h"
#include "ConstraintList.h"
#include "Architecture/Collider.h"
#include "Architecture/RigidBody.h"
#include "PhysicsUtil.h"

namespace WickedSick
{
  ContactManifold::ContactManifold(Collider* first, Collider* second) : iteration_(false), 
                                                                        first_(first), 
                                                                        second_(second)
  {
  }

  ContactManifold::~ContactManifold()
  {
    for(auto it : constraints_.Get())
    {
      it->SetSatisfied(true);
    }
    for(auto& it : contacts_)
    {
      delete it;
      it = nullptr;
    }
  }

  void ContactManifold::AddContact(Contact* contact)
  {
    if(contacts_.size() < 4)
    {
      for(unsigned i = 0; i < contacts_.size(); ++i)
      {
        if((contact->GetWorldContactPoint1() - contacts_[i]->GetWorldContactPoint1()).LengthSq() <= 1.0f)
        {
          return;
        }
      }
      contacts_.push_back(new Contact(*contact));
      if(first_->Resolves() && second_->Resolves())
      {
        constraints_.push(std::shared_ptr<ContactConstraint>(new ContactConstraint(first_, second_, contacts_.back())));
        constraints_.back()->Initialize();
        first_->GetBase()->GetConstraints().push(constraints_.back());
        second_->GetBase()->GetConstraints().push(constraints_.back());
      }
    }
    else //process of adding new contact. long and tedious. can be optimized.
    {
      //find farthest point from the first point
      Contact* farthest = contact;
      float lengthnew = (farthest->GetWorldContactPoint1() - contacts_[0]->GetWorldContactPoint1()).LengthSq();
      float lengthone = (contacts_[1]->GetWorldContactPoint1() - contacts_[0]->GetWorldContactPoint1()).LengthSq();
      float lengthtwo = (contacts_[2]->GetWorldContactPoint1() - contacts_[0]->GetWorldContactPoint1()).LengthSq();
      float lengththree = (contacts_[3]->GetWorldContactPoint1() - contacts_[0]->GetWorldContactPoint1()).LengthSq();

      float longest = std::max(lengthnew, std::max(lengthone, std::max(lengthtwo, lengththree)));

      if(longest == lengthone)
      {
        farthest = contacts_[1];
        //second contact is already set
      }
      else if(longest == lengthtwo)
      {
        farthest = contacts_[2];
        std::swap(*contacts_[1], *contacts_[2]);
      }
      else if(longest == lengththree)
      {
        farthest = contacts_[3];
        std::swap(*contacts_[1], *contacts_[3]);
      }
      else
      {
        farthest = contact;
        std::swap(*contacts_[1], *contact);
      }

      //now we must find the farthest point from the line formed by the first two
      lengthone   = PointToLineDistSq(contacts_[0]->GetWorldContactPoint1(), farthest->GetWorldContactPoint1(), contacts_[2]->GetWorldContactPoint1());
      lengthtwo   = PointToLineDistSq(contacts_[0]->GetWorldContactPoint1(), farthest->GetWorldContactPoint1(), contacts_[3]->GetWorldContactPoint1());
      lengththree = PointToLineDistSq(contacts_[0]->GetWorldContactPoint1(), farthest->GetWorldContactPoint1(), contact->GetWorldContactPoint1());

      longest = std::max(lengthone, std::max(lengthtwo,lengththree));
      if(longest == lengthone)
      {
        farthest = contacts_[2];
        //third contact is already set
      }
      else if(longest == lengthtwo)
      {
        farthest = contacts_[3];
        std::swap(*contacts_[2], *contacts_[3]);
      }
      else
      {
        farthest = contact;
        std::swap(*contacts_[2], *contact);
      }

      //finally, find the point farthest from the triangle formed by the three others
      lengthone = PointToTriSq(contacts_[0]->GetWorldContactPoint1(), contacts_[1]->GetWorldContactPoint1(), contacts_[2]->GetWorldContactPoint1(), contacts_[3]->GetWorldContactPoint1());
      lengthtwo = PointToTriSq(contacts_[0]->GetWorldContactPoint1(), contacts_[1]->GetWorldContactPoint1(), contacts_[2]->GetWorldContactPoint1(), contact->GetWorldContactPoint1());

      longest = std::max(lengthone, lengthtwo);

      if(longest == 0.0f)
      {
        return;
      }

      if(longest == lengthone)
      {
        //alright, we have our fourth contact already.
        return;
      }
      else
      {
        std::swap(*contacts_[3], *contact);
      }
    }
  }

  bool ContactManifold::GetIteration() 
  { 
    return iteration_; 
  }
  
  void ContactManifold::ToggleIteration() 
  { 
    iteration_ = !iteration_; 
  }

  int ContactManifold::GetNumContacts() 
  { 
    return contacts_.size(); 
  }

  std::vector<Contact*>& ContactManifold::GetContacts() 
  { 
    return contacts_; 
  }
}
