#pragma once
//

namespace Reflection
{
  template <typename RetType>
  ReturnType<RetType>::ReturnType()
  {
  }

  template <typename RetType>
  ReturnType<RetType>::ReturnType(RetTypeFinal&& in)
  {
    RetTypeFinal* target = reinterpret_cast<RetTypeFinal*>(object);
    memcpy((void*)object, (void*)&in, sizeof(in));
  }

  template <typename RetType>
  ReturnType<RetType>::ReturnType(const RetTypeFinal& in)
  {
    memcpy((void*)object, (void*)&in, sizeof(in));
  }

  template <typename RetType>
  void ReturnType<RetType>::Set(RetTypeFinal&& in)
  {
    memcpy((void*)object, (void*)&in, sizeof(in));
  }

  template <typename RetType>
  void ReturnType<RetType>::Set(const RetTypeFinal& in)
  {
    memcpy((void*)object, (void*)&in, sizeof(in));
  }

  template <typename RetType>
  void* ReturnType<RetType>::Get() const
  {
    return (void*)object;
  }

}
