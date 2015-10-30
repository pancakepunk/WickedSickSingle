#pragma once

#include <vector>

#include "meta/Utility/IndexSequence.h"
namespace Reflection
{
  class Var;
  class Metadata;

  class Constructor
  {
  public:
    typedef void(*CtorPtr)(Var* returnValue, std::vector<Var*> args);

    Constructor();
    ~Constructor();

    std::vector<Metadata*>& GetArgs();

    void operator()(Var* returnValue, std::vector<Var*> args);

    template<typename ClassType, typename ...ArgType>
    Constructor(const ClassType&, const ArgType&...);

    template<typename ClassType>
    Constructor(const ClassType&);


    template <typename ClassType, typename ...ArgType>
    class FunctionData
    {
    public:

      static void Call(Var* returnValue, std::vector<Var*> args);

      template<std::size_t ...Is>
      static void CallFinal(Var* returnValue,
                            std::vector<Var*> args,
                            index_sequence<Is...>);

      template<typename ...ArgTypes>
      static void CallActualFinal(Var* returnValue,
                                  ArgTypes*... args);
    };

    template <typename ClassType>
    class FunctionData<ClassType>
    {
    public:

      static void Call(Var* returnValue, std::vector<Var*> args);
    };


    CtorPtr caller;
    std::vector<Metadata*> args_;
  };



}
#include "ConstructorTemplate.cpp"
