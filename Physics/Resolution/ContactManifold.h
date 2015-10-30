#pragma once

#include "ConstraintList.h"

namespace WickedSick
{
  class Contact;
  class ContactEdge;
  class Collider;
  class ContactManifold
  {
    public:
      ContactManifold(Collider* first, Collider* second);
      ~ContactManifold();
      void AddContact(Contact* contact);
      bool GetIteration();
      void ToggleIteration();
      int GetNumContacts();
      std::vector<Contact*>& GetContacts();

    private:
      bool     iteration_;
      std::vector<Contact*> contacts_;
      ConstraintList constraints_;
      Collider* first_;
      Collider* second_;

  };
}

