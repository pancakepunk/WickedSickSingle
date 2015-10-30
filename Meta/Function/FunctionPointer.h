#pragma once

namespace Reflection
{
  struct GenericBase1 {};
  struct GenericBase2 {};
  struct GenericDerviedClass : public GenericBase1 {};
  struct GenericClass {};
  struct GenericUnknownClass;

  typedef void (GenericUnknownClass::*UnknownClassFn)       (void);
  typedef void (GenericDerviedClass::*GenericDerivedFn)     (void);
  typedef void (GenericClass::*GenericClassFn)              (void);
  typedef void(*StaticFn)                                  (void);


  union FunctionPointer//union used to reference a function pointer with any signature, and interpret it as any signature function pointer
  {
    UnknownClassFn   unknownClassFn;    //for any unknown class
    GenericDerivedFn genericDerivedFn;  //for any derived
    GenericClassFn   genericClassFn;
    StaticFn         staticFn;          //for any global
  };

}
