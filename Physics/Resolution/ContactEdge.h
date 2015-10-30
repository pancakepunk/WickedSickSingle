#pragma once

namespace WickedSick
{
  class ContactManifold;
  class Collider;

  enum EdgeType
  {
    ET_Static,
    ET_Dynamic,
    ET_Count
  };

  class ContactEdge
  {
    public:
      ContactEdge(Collider* self, Collider* other);
      ~ContactEdge();
      void SetManifold(ContactManifold* manifold){ manifold_ = manifold; }
      void SetOther(Collider* other){ other_ = other; }
      ContactManifold* GetManifold(){ return manifold_; }
      Collider* GetOther(){ return other_; }
      Collider* GetSelf(){ return self_; }
      EdgeType GetType(){ return type_; }
    private:
      ContactManifold* manifold_;
      float velocities_[1][12];
      Collider* self_;
      Collider* other_;
      EdgeType type_;
  };
}

