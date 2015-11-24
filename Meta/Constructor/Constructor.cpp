#include "Precompiled.h"//
#include "meta/Constructor/Constructor.h"

#include "meta/Utility/MetaUtility.h"
#include "meta/Utility/SimplifyType.h"
namespace Reflection
{
  Constructor::Constructor()
  {
  }

  Constructor::~Constructor()
  {
  }

  std::vector<Metadata*>& Constructor::GetArgs()
  {
    return args_;
  }

  void Constructor::operator()(Var* returnValue, std::vector<Var*> args)
  {
    caller(returnValue, args);
  }
}
