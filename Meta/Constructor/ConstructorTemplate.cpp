//
#include "meta/Constructor/Constructor.h"
#include "meta/Variant/Variant.h"

namespace Reflection
{
  template<typename ClassType, typename ...ArgType>
  Constructor::Constructor(const ClassType&, const ArgType&...)
  {
    caller = &FunctionData<ClassType, ArgType...>::Call;

    if (sizeof...(ArgType))
    {
      auto list = { FindType(ArgType)... };
      args_ = list;
    }
  }

  template<typename ClassType>
  Constructor::Constructor(const ClassType&)
  {
    caller = &FunctionData<ClassType>::Call;
  }



  template<typename ClassType, typename ...ArgType>
  void Constructor::FunctionData<ClassType, ArgType...>::Call(Var * returnValue,
                                                              std::vector<Var*> args)
  {
    auto list = { FindType(ArgType)... };

    if ((args.size() != list.size()) &&
        ((*list.begin()) != FindType(void)))
    {
      __debugbreak();
    }

    CallFinal(returnValue, args, make_index_sequence<sizeof...(ArgType)>());
  }

  template<typename ClassType, typename ...ArgType>
  template<std::size_t ...Is>
  void Constructor::FunctionData<ClassType, ArgType...>::CallFinal(Var * returnValue,
                                                                   std::vector<Var*> args,
                                                                   index_sequence<Is...>)
  {
    if (!args.empty())
    {
      auto allocatedList = { SetArg<ArgType>(args[Is], alloca(sizeof(ArgType))) ... };
      std::vector<void*> finalArgList = allocatedList;
      CallActualFinal(returnValue, ((RemoveReferences(ArgType)*)(finalArgList[Is])) ...);

    }
  }

  template<typename ClassType, typename ...ArgType>
  template<typename ...ArgTypes>
  static void Constructor::FunctionData<ClassType, ArgType...>::CallActualFinal(Var* returnValue,
                                                                                ArgTypes*... args)
  {
    void* data = returnValue->GetData();
    if (!data)
    {
      __debugbreak();
    }
    new (returnValue->GetData()) ClassType((*args)...);
  }


  template<typename ClassType>
  void Constructor::FunctionData<ClassType>::Call(Var * returnValue, std::vector<Var*> args)
  {
    new (returnValue->GetData()) ClassType();
  }
}
