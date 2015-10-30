#pragma once

namespace Reflection
{

  template <typename T>
  struct Simplify
  {
    typedef T Type;
  };

  template <typename T>
  struct Simplify<const T>
  {
    typedef typename Simplify<T>::Type Type;
  };

  template <typename T>
  struct Simplify<T&>
  {
    typedef typename Simplify<T>::Type Type;
  };

  template <typename T>
  struct Simplify<T&&>
  {
    typedef typename Simplify<T>::Type Type;
  };

  template <typename T>
  struct Simplify<T*>
  {
    typedef typename Simplify<T>::Type Type;
  };

  template <typename T, int index>
  struct Simplify<T[index]>
  {
    typedef typename Simplify<T>::Type Type;
  };

  template <typename T>
  struct RemoveRefs
  {
    typedef T Type;
  };

  template <typename T>
  struct RemoveRefs<T&>
  {
    typedef typename RemoveRefs<T>::Type Type;
  };

  template <typename T>
  struct RemoveRefs<T&&>
  {
    typedef typename RemoveRefs<T>::Type Type;
  };

  template <typename T>
  struct RemoveRefs<const T&>
  {
    typedef typename RemoveRefs<T>::Type Type;
  };

  template <typename T>
  struct RemoveRefs<const T&&>
  {
    typedef typename RemoveRefs<T>::Type Type;
  };


  template <typename T>
  struct RemoveConst
  {
    typedef T Type;
  };

  template <typename T>
  struct RemoveConst<const T&>
  {
    typedef typename RemoveConst<T>::Type Type;
  };
}

#define SimplifyType(T) Reflection::Simplify<T>::Type
#define RemoveReferences(T) Reflection::RemoveRefs<T>::Type
