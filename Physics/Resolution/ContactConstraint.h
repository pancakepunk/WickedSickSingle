#pragma once
#include "Constraint.h"
namespace WickedSick
{
  class Collider;

  class Contact;
  class RigidBody;
  class ContactConstraint : public Constraint
  {
    public:
      ContactConstraint(Collider* first, 
                        Collider* second,
                        Contact* contact);
      ~ContactConstraint();
      void Iterate(const float& dt);
      void Initialize();
    private:
      Contact* contact_;
      Collider* first_collider_;
      Collider* second_collider_;
      RigidBody* first_body_;
      RigidBody* second_body_;
      float restitution_;
      float restitution_term_;
      float warm_start_;
      const float restitution_slop_; // half a meter per second
      const float penetration_slop_; // five millimeters
      const float baumgarte_term_;
  };
}

