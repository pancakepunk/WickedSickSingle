#pragma once

namespace Reflection
{

  template<typename T>
  struct IndirectionCount
  {
    static const unsigned short value = 0;
  };

  template<typename U>
  struct IndirectionCount<U*>
  {
    static const unsigned short value = 1 + IndirectionCount<U>::value;
  };

  template<typename T>
  struct RefCount
  {
    static const unsigned short value = 0;
  };

  template<typename U>
  struct RefCount<U&>
  {
    static const unsigned short value = 1 + RefCount<U>::value;
  };



  void* blind_dereference(void* ptr, int derefs = 1);


}

