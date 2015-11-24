#include "Precompiled.h"
#include "Precompiled.h"
//
#include "meta/Type/Member.h"
namespace Reflection
{
  Member::Member(void) : owner_(nullptr),
    type_(nullptr),
    offset_(0),
    indirection_(0)
  {

  }

  Member::Member(std::string name,
                 Metadata * owner,
                 Metadata * type,
                 unsigned offset,
                 unsigned indirection)
    : name_(name),
    owner_(owner),
    type_(type),
    offset_(offset),
    indirection_(indirection)
  {

  }

  const std::string & Member::GetName() const
  {
    return name_;
  }

  Metadata * Member::GetOwner()
  {
    return owner_;
  }

  Metadata * Member::GetType()
  {
    return type_;
  }

  unsigned Member::GetOffset()
  {
    return offset_;
  }

  unsigned Member::GetIndirection()
  {
    return indirection_;
  }
}
