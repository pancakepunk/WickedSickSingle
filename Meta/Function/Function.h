#pragma once



#include "meta/Function/FunctionPointer.h"
#include "meta/Function/ReturnType.h"
#include "meta/Utility/IndexSequence.h"
#include "meta/Utility/IndirectionCount.h"
#include "meta/Utility/SimplifyType.h"
#include <vector>


#define ARGTYPE_FINAL(arg) typedef SIMPLIFY_TYPE<arg> arg##Final

namespace Reflection
{

  class Var;
  class Metadata;
  class Function
  {
  public:

    Function();
    ~Function();

    void operator()(Var* object, Var* returnValue, std::vector<Var*> args);

    template<typename ClassType, typename RetType, typename ...ArgType>
    Function(RetType (ClassType::*function)(ArgType...) volatile);

    template<typename ClassType, typename RetType, typename ...ArgType>
    Function(RetType (ClassType::*function)(ArgType...) const);

    template<typename ClassType, typename RetType, typename ...ArgType>
    Function(RetType (ClassType::*function)(ArgType...));

    template<typename ClassType, typename RetType>
    Function (RetType (ClassType::*function)() volatile);

    template<typename ClassType, typename RetType>
    Function (RetType (ClassType::*function)() const);

    template<typename ClassType, typename RetType>
    Function (RetType (ClassType::*function)());

    

    template <typename ClassType, typename RetType, typename ...ArgType>
    class FunctionData
    {
    public:

      typedef RetType (ClassType::*Method)(ArgType...);

      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...) volatile);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...) const);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...));

      static void Call(FunctionPointer& ptr, 
                       Var* object, 
                       Var* returnValue, 
                       std::vector<Var*> args);
    

      template<std::size_t ...Is>
      static void CallFinal(ReturnType<RetType>& toFill,
                            FunctionPointer& ptr, 
                            ClassType* c, 
                            std::vector<Var*> args, 
                            index_sequence<Is...>);
    
      template<typename ...ArgTypes>
      static RetType CallActualFinal(ClassType* c,
                                     Method method,
                                     ArgTypes*... args);

      /*template<typename ...ArgTypes>
      static RetType CallActualFinal(ClassType* c,
                                     Method method,
                                     void* badArg,
                                     ArgTypes*... args);

      template<>
      static RetType CallActualFinal(ClassType* c,
                                     Method method);*/
    };

    template <typename ClassType, typename RetType, typename ...ArgType>
    class FunctionData<ClassType, RetType, void, ArgType...>
    {
    public:


      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...) volatile);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...) const);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...));

      static void Call(FunctionPointer& ptr, 
                       Var* object, 
                       Var* returnValue, 
                       std::vector<Var*> args);
    };

  

    template <typename ClassType, typename RetType>
    class FunctionData<ClassType, RetType>
    {
    public:
      typedef typename RemoveReferences(RetType) RetTypeFinal;
      typedef RetType (ClassType::*Method)();
    

      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)() volatile);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)() const);
      static void Set(FunctionPointer& ptr, RetType (ClassType::*function)());

      static void Call(FunctionPointer& ptr, 
                       Var* object, 
                       Var* returnValue, 
                       std::vector<Var*> args);
    };


    template <typename ClassType, typename ...ArgType>
    class FunctionData<ClassType, void, ArgType...>
    {
    public:
      typedef void (ClassType::*Method)(ArgType...);

      static void Set(FunctionPointer& ptr, void (ClassType::*function)(ArgType...) volatile);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)(ArgType...) const);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)(ArgType...));

      static void Call(FunctionPointer& ptr, Var* object, Var* returnValue, std::vector<Var*> args);

      template<std::size_t ...Is>
      static void CallFinal(FunctionPointer& ptr, 
                            ClassType* c, 
                            std::vector<Var*> args, 
                            index_sequence<Is...>);


      template<typename ...ArgTypes>
      static void CallActualFinal(ClassType* c,
                                  Method method,
                                  ArgTypes*... args);

      //template<typename ...ArgTypes>
      //static void CallActualFinal(ClassType* c,
      //                            Method method,
      //                            void* badArg,
      //                            ArgTypes*... args);

      //template<>
      //static void CallActualFinal(ClassType* c,
      //                            Method method);
    };
 

    template <typename ClassType, typename ...ArgTypes>
    class FunctionData<ClassType, void, void, ArgTypes...>
    {
    public:
      typedef void (ClassType::*Method)();
      static void Set(FunctionPointer& ptr, void (ClassType::*function)() volatile);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)() const);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)());

      static void Call(FunctionPointer& ptr, Var* object, Var* returnValue, std::vector<Var*> args);
    };
  
    template <typename ClassType>
    class FunctionData<ClassType, void>
    {
    public:
      typedef void (ClassType::*Method)();

      static void Set(FunctionPointer& ptr, void (ClassType::*function)() volatile);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)() const);
      static void Set(FunctionPointer& ptr, void (ClassType::*function)());

      static void Call(FunctionPointer& ptr, Var* object, Var* returnValue, std::vector<Var*> args);

    };

    void (*caller)(FunctionPointer& ptr, 
                   Var* object, 
                   Var* returnValue, 
                   std::vector<Var*> args);
    Metadata* return_type_;
    std::vector<Metadata*> args_;
    FunctionPointer        function_;
  };

}
#include "FunctionTemplate.cpp"
