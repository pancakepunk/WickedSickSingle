#pragma once

namespace Reflection
{

  class Metadata;
  class Member
  {
  public:
    Member(void);

    Member(std::string name,
           Metadata* owner,
           Metadata* type,
           unsigned offset,
           unsigned indirection);

    const std::string& GetName() const;

    Metadata* GetOwner();

    Metadata* GetType();

    unsigned GetOffset();

    unsigned GetIndirection();

  private:
    std::string name_;
    Metadata* owner_;
    Metadata* type_;
    unsigned indirection_;
    unsigned offset_;
  };
}
