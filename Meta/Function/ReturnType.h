#pragma once

#include "meta/Utility/SimplifyType.h"

namespace Reflection
{
  template<typename RetType>
  struct ReturnType
  {
    typedef typename RemoveReferences(RetType) RetTypeFinal;

    ReturnType();
    ReturnType(RetTypeFinal&& in);
    ReturnType(const RetTypeFinal& in);

    void Set(RetTypeFinal&& in);
    void Set(const RetTypeFinal& in);

    void* Get() const;

    char object[sizeof(RetType)];
  };
}



#include "ReturnType.cpp"
