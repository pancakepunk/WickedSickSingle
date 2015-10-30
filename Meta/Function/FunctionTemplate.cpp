#pragma once

//


#include "meta/Utility/MetaUtility.h"
#include "meta/Type/MetaManager.h"

namespace Reflection
{
  template <typename ClassType, typename RetType, typename ...ArgType>
  Function::Function(RetType(ClassType::*function)(ArgType...) volatile) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, ArgType...>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, ArgType...>::Call;

    if (sizeof...(ArgType))
    {
      auto list = { FindType(ArgType)... };
      args_ = list;
    }

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  Function::Function(RetType(ClassType::*function)(ArgType...) const) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, ArgType...>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, ArgType...>::Call;

    if (sizeof...(ArgType))
    {
      auto list = { FindType(ArgType)... };
      args_ = list;
    }

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  Function::Function(RetType(ClassType::*function)(ArgType...)) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, ArgType...>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, ArgType...>::Call;

    if (sizeof...(ArgType))
    {
      auto list = { FindType(ArgType)... };
      args_ = list;
    }

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }

  template <typename ClassType, typename RetType>
  Function::Function(RetType(ClassType::*function)() volatile) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, void>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, void>::Call;

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }

  template <typename ClassType, typename RetType>
  Function::Function(RetType(ClassType::*function)() const) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, void>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, void>::Call;

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }

  template <typename ClassType, typename RetType>
  Function::Function(RetType(ClassType::*function)()) : return_type_(nullptr)
  {
    FunctionData<ClassType, RetType, void>::Set(function_, function);
    caller = &FunctionData<ClassType, RetType, void>::Call;

    if (!std::is_void<RetType>())
    {
      return_type_ = FindType(RetType);
    }
  }



  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData<ClassType, RetType, ArgType...>::Set(FunctionPointer& ptr, 
                                          RetType(ClassType::*function)(ArgType...) volatile)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData<ClassType, RetType, ArgType...>::Set(FunctionPointer& ptr,
                                          RetType(ClassType::*function)(ArgType...) const)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData<ClassType, RetType, ArgType...>::Set(FunctionPointer& ptr,
                                          RetType(ClassType::*function)(ArgType...))
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }


  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData<ClassType, RetType, ArgType...>::Call(FunctionPointer& ptr,
                                                                    Var* object,
                                                                    Var* returnValue,
                                                                    std::vector<Var*> args)
  {
    auto argTypeList = { FindType(ArgType)... };

    if ((args.size() != argTypeList.size()) &&
        (*(argTypeList.begin()) != FindType(void)))
    {
      __debugbreak();
    }

    ClassType* c = reinterpret_cast<ClassType*>(blind_dereference(object->GetData(), object->GetIndirection()));

    Metadata* finalMeta = FindType(RemoveRefs<RetType>::Type);
    ReturnType<RetType> retType;

    CallFinal(retType,
              ptr,
              c,
              args,
              make_index_sequence<sizeof...(ArgType)>());

    const unsigned short returnInd = IndirectionCount<RemoveRefs<RetType>::Type>::value;

    returnValue->SetLevelsOfIndirection(returnInd);
    returnValue->SetValueIndirection(retType.Get(), returnInd + 1);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  template<std::size_t ...Is>
  void Function::FunctionData<ClassType, RetType, ArgType...>::CallFinal(ReturnType<RetType>& toFill,
                                                                         FunctionPointer& ptr,
                                                                         ClassType* c,
                                                                         std::vector<Var*> args,
                                                                         index_sequence<Is...>)
  {
    typedef typename RemoveReferences(RetType) RetTypeFinal;
    //typedef RetType (ClassType::*Method)(ArgType...);
    Method m = (Method)ptr.genericClassFn;

    if (!args.empty())
    {
      std::vector<unsigned short> indirectionList = { IndirectionCount<ArgType>::value ... };

      std::vector<void*> allocatedList = { (void*)alloca(sizeof(ArgType)) ... };

      //allocate enough room on the stack for the list of args
      void* currentLevel;
      int allocations;
      for (unsigned i = 0; i < indirectionList.size(); ++i)
      {
        allocations = indirectionList[i];
        currentLevel = allocatedList[i];
        while (allocations)
        {
          if (allocations == 1)
          {
            *reinterpret_cast<void**>(currentLevel) = alloca(args[i]->GetMetadata()->GetSize());
          }
          else
          {
            *reinterpret_cast<void**>(currentLevel) = alloca(sizeof(void*));
          }
          currentLevel = *reinterpret_cast<void**>(currentLevel);
          --allocations;
        }
      }
      std::vector<bool> destructList = { args[Is]->GetMetatype() != FindType(ArgType) ... };
      std::vector<void*> finalArgList = { SetArg<ArgType>(args[Is], allocatedList[Is]) ... };

      toFill.Set(CallActualFinal(c, m, ((RemoveReferences(ArgType)*)(finalArgList[Is])) ...));

      Destruct((RemoveReferences(ArgType)*)allocatedList[Is] ...);

    }
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  template<typename ...ArgTypes>
  RetType Function::FunctionData<ClassType, RetType, ArgType...>::CallActualFinal(ClassType* c,
                                                                                  Method method,
                                                                                  ArgTypes*... args)
  {
    return (c->*method)((*args)...);
  }

  //template <typename ClassType, typename RetType, typename ...ArgType>
  //template<typename ...ArgTypes>
  //static RetType Function::FunctionData<ClassType, RetType, ArgType...>::CallActualFinal(ClassType* c,
  //                                                       Method method,
  //                                                       void* badArg,
  //                                                       ArgTypes*... args)
  //{
  //  return CallActualFinal(c, method, args...);
  //}
  //
  //template <typename ClassType, typename RetType, typename ...ArgType>
  //template<>
  //static RetType Function::FunctionData<ClassType, RetType, ArgType...>::CallActualFinal<void>( ClassType* c,
  //                                                        Method method)
  //{
  //  return (c->*method)();
  //}




  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData <ClassType, RetType, void, ArgType...>::Set(FunctionPointer& ptr, 
                                                                          RetType(ClassType::*function)(ArgType...) volatile)
  {
    FunctionData <ClassType, RetType, ArgType...>::Set(ptr, function);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData <ClassType, RetType, void, ArgType...>::Set(FunctionPointer& ptr,
                                                                          RetType(ClassType::*function)(ArgType...) const)
  {
    FunctionData <ClassType, RetType, ArgType...>::Set(ptr, function);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData <ClassType, RetType, void, ArgType...>::Set(FunctionPointer& ptr,
                                                                          RetType(ClassType::*function)(ArgType...))
  {
    FunctionData <ClassType, RetType, ArgType...>::Set(ptr, function);
  }

  template <typename ClassType, typename RetType, typename ...ArgType>
  void Function::FunctionData <ClassType, RetType, void, ArgType...>::Call(FunctionPointer& ptr, 
                                                                           Var* object,
                                                                           Var* returnValue,
                                                                           std::vector<Var*> args)
  {
    FunctionData<ClassType, RetType, ArgType...>::Call(ptr, 
                                                       object, 
                                                       returnValue, 
                                                       args);
  }

  template <typename ClassType, typename RetType>
  void Function::FunctionData<ClassType, RetType>::Set(FunctionPointer& ptr, 
                                                              RetType(ClassType::*function)() volatile)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename RetType>
  void Function::FunctionData<ClassType, RetType>::Set( FunctionPointer& ptr,
                                                        RetType(ClassType::*function)() const)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename RetType>
  void Function::FunctionData<ClassType, RetType>::Set( FunctionPointer& ptr,
                                                        RetType(ClassType::*function)())
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename RetType>
  void Function::FunctionData<ClassType, RetType>::Call(FunctionPointer& ptr,
                                                        Var* object,
                                                        Var* returnValue,
                                                        std::vector<Var*> args)
  {
    ClassType* c = reinterpret_cast<ClassType*>(blind_dereference(object->GetData(), object->GetIndirection()));

    Metadata* finalMeta = FindType(RemoveRefs<RetType>::Type);

    Method m = (Method)ptr.genericClassFn;

    const unsigned short returnInd = IndirectionCount<RemoveRefs<RetType>::Type>::value;

    returnValue->SetLevelsOfIndirection(returnInd);

    ReturnType<RetType> result((c->*m)());

    memcpy(returnValue->GetData(), result.Get(), sizeof(RemoveRefs<RetType>::Type));
  }

  template <typename ClassType, typename ...ArgType>
  void Function::FunctionData<ClassType, void, ArgType...>::Set( FunctionPointer& ptr, 
                                                                        void (ClassType::*function)(ArgType...) volatile)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename ...ArgType>
  void Function::FunctionData<ClassType, void, ArgType...>::Set( FunctionPointer& ptr,
                                                                        void (ClassType::*function)(ArgType...) const)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename ...ArgType>
  void Function::FunctionData<ClassType, void, ArgType...>::Set( FunctionPointer& ptr,
                                                                        void (ClassType::*function)(ArgType...))
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType, typename ...ArgType>
  void Function::FunctionData<ClassType, void, ArgType...>::Call(FunctionPointer& ptr, 
                                                                        Var* object,
                                                                        Var* returnValue,
                                                                        std::vector<Var*> args)
  {
    auto argTypeList = { FindType(ArgType)... };

    if ((args.size() != argTypeList.size()) &&
        (*(argTypeList.begin()) != FindType(void)))
    {
      __debugbreak();
    }

    ClassType* c = reinterpret_cast<ClassType*>(blind_dereference(object->GetData(), object->GetIndirection()));

    CallFinal(ptr,
              c,
              args,
              make_index_sequence<sizeof...(ArgType)>());

  }

  template <typename ClassType, typename ...ArgType>
  template<std::size_t ...Is>
  void Function::FunctionData<ClassType, void, ArgType...>::CallFinal( FunctionPointer& ptr,
                                                                              ClassType* c,
                                                                              std::vector<Var*> args,
                                                                              index_sequence<Is...>)
  {
    Method m = (Method)ptr.genericClassFn;

    auto allocatedList = { SetArg<ArgType>(args[Is], alloca(sizeof(ArgType))) ... };
    std::vector<void*> finalArgList = allocatedList;
    CallActualFinal(c, m, ((RemoveReferences(ArgType)*)(finalArgList[Is])) ...);
  }

  template <typename ClassType, typename ...ArgType>
  template<typename ...ArgTypes>
  void Function::FunctionData<ClassType, void, ArgType...>::CallActualFinal( ClassType* c,
                                                                                    Method method,
                                                                                    ArgTypes*... args)
  {
    return (c->*method)((*args)...);
  }

  //template <typename ClassType, typename ...ArgType>
  //template<typename ...ArgTypes>
  //void Function::FunctionData<ClassType, void, ArgType...>::CallActualFinal(ClassType* c,
  //                                                                                 Method method,
  //                                                                                 void* badArg,
  //                                                                                 ArgTypes*... args)
  //{
  //  return CallActualFinal(c, method, args...);
  //}
  //
  //template <typename ClassType, typename ...ArgType>
  //template <>
  //void Function::FunctionData<ClassType, void, ArgType...>::CallActualFinal<void>(ClassType* c,
  //                                                                                 Method method)
  //{
  //  return (c->*method)();
  //}


  template <typename ClassType, typename ...ArgTypes>
  void Function::FunctionData<ClassType, void, void, ArgTypes...>::Set(FunctionPointer& ptr, 
                                                                              void (ClassType::*function)() volatile)
  {
    FunctionData<ClassType, void, ArgTypes...>::Set(ptr, function);
  }

  template <typename ClassType, typename ...ArgTypes>
  void Function::FunctionData<ClassType, void, void, ArgTypes...>::Set(FunctionPointer& ptr,
                                                                              void (ClassType::*function)() const)
  {
    FunctionData<ClassType, void, ArgTypes...>::Set(ptr, function);
  }

  template <typename ClassType, typename ...ArgTypes>
  void Function::FunctionData<ClassType, void, void, ArgTypes...>::Set(FunctionPointer& ptr,
                                                                              void (ClassType::*function)())
  {
    FunctionData<ClassType, void, ArgTypes...>::Set(ptr, function);
  }

  template <typename ClassType, typename ...ArgTypes>
  void Function::FunctionData<ClassType, void, void, ArgTypes...>::Call(FunctionPointer& ptr, 
                                                                               Var* object, 
                                                                               Var* returnValue, 
                                                                               std::vector<Var*> args)
  {
    FunctionData<ClassType, void, ArgTypes...>::Call(ptr, object, returnValue, args);
  }

  template <typename ClassType>
  void Function::FunctionData<ClassType, void>::Set(FunctionPointer& ptr, 
                                                           void (ClassType::*function)() volatile)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType>
  void Function::FunctionData<ClassType, void>::Set(FunctionPointer& ptr,
                                                           void (ClassType::*function)() const)
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType>
  void Function::FunctionData<ClassType, void>::Set(FunctionPointer& ptr,
                                                           void (ClassType::*function)())
  {
    ptr.genericClassFn = reinterpret_cast<GenericClassFn>(function);
  }

  template <typename ClassType>
  void Function::FunctionData<ClassType, void>::Call(FunctionPointer& ptr, 
                                                            Var* object, 
                                                            Var* returnValue, 
                                                            std::vector<Var*> args)
  {
    Method m = (Method)ptr.genericClassFn;
    ClassType* c = reinterpret_cast<ClassType*>(blind_dereference(object->GetData(), object->GetIndirection()));
    (c->*m)();
  }
}