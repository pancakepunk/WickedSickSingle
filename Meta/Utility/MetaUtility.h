
#pragma once
//#include "GameObject.h"
#include "meta/Utility/SimplifyType.h"
#include "meta/Function/FunctionPointer.h"
#include "meta/Lua/LuaUserdata.h"
#include "meta/Variant/Variant.h"
#include "meta/Variant/VariantTemplate.h"
#include "stdint.h" /* Replace with <stdint.h> if appropriate */
namespace Reflection
{
  
  #undef get16bits
  #if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) \
    || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
  #define get16bits(d) (*((const uint16_t *) (d)))
  #endif

  #if !defined (get16bits)
  #define get16bits(d) ((((uint32_t)(((const uint8_t *)(d))[1])) << 8)\
                         +(uint32_t)(((const uint8_t *)(d))[0]) )
  #endif

  uint32_t SuperFastHash(const char * data, int len);

  template<typename From, typename To>
  void SetValue()
  {

  }


  //template <typename ClassType, typename RetType, typename ...Args>
  //RetType(ClassType::*)(RemoveReferences(Args)...) ResolveFunction(FunctionPointer& ptr, RetType (ClassType::*function)(ArgType...))
  //{
  //  RetType (ClassType::*tempPtr)(RemoveReferences(ArgType)...);
  //  tempPtr = reinterpret_cast<decltype(tempPtr)>(ptr.genericClassFn);
  //  return tempPtr;
  //}

  void* resolveIndirection(int from, int to, void* fromPtr, void* toPtr);

  template<typename Last>
  void Destruct(Last* last)
  {
    last->~Last();
  }

  template<typename First, typename ...Rest>
  void Destruct(First* first, Rest*... rest)
  {
    first->~First();
    Destruct(rest...);
  }


  template <typename Type>
  void* SetArg(Var* arg, void* argMemory)
  {
    typedef std::remove_reference<Type>::type PassType;
    Metadata* argMeta = arg->GetMetadata();
    Metadata* passMeta = FindType(PassType);
    int passIndirection = IndirectionCount<PassType>::value + 1;// we add 1 because we need a pointer to our pass type by the end

    if ((argMeta == FindType(char)) &&
        (arg->GetIndirection() == 1))
    {
      //return arg->GetData();
      if (passMeta == FindType(std::string))
      {
        new(argMemory) std::string((char*)arg->GetData());
        return argMemory;
      }
      else if ((passMeta == FindType(char)) &&
               (passIndirection == 1))
      {
        return arg->GetData();
      }
    }


    PassType* objectToPass = static_cast<PassType*>(argMemory);

    LuaUserdata* userdata = reinterpret_cast<LuaUserdata*>(arg->GetData());

    bool variantMatches = (userdata->GetType() == arg->GetMetadata());


    switch (passMeta->GetValueType())
    {
      case ByReference:
      {
        if (argMeta == passMeta ||
            argMeta->InheritsFrom(passMeta))
        {
          return resolveIndirection(arg->GetIndirection() + 1, // we + 1 here because arg->GetData is a pointer, adding 1 more
                                    passIndirection,
                                    arg->GetData(),
                                    argMemory);
        }
        __debugbreak();// this is probably bad.
        break;
      }
      case ByValue:
      {
        //here we'll be working with the memory given to us
        if (argMeta == passMeta ||
            argMeta->Converts(passMeta))
        {
          void* argValue = argMemory;
          int indirectionIter = passIndirection;
          while (indirectionIter > 1)
          {
            argValue = *reinterpret_cast<void**>(argValue);
            --indirectionIter;
          }


          //this function is gross. i want to kill it.
          GenericSetValue(argMeta,
                          arg->GetData(),
                          passMeta,
                          argValue);

          return resolveIndirection(indirectionIter,
                                    passIndirection,
                                    argValue,
                                    argMemory);
        }
        __debugbreak();
        break;
      }
      default:
        __debugbreak();//by pointer or by none?
    }
    return nullptr;
  }

  unsigned int MetaHash(Metadata* meta, void* object);

  void StringToVal(std::string input, Metadata* outType, void* out);

  std::string GetStringOf(Metadata* outType, void* out);

  template<typename T>
  T GetEnumByName(const std::string& name)
  {
    std::vector<const char*>& enums = FindType(T)->GetEnumConstants();
    for (unsigned i = 0; i < enums.size(); ++i)
    {
      if (enums[i] && (name == enums[i]))
      {
        return static_cast<T>(i);
      }
    }
    return (T)-1;
  }

  void GenericSetValue(Metadata* startType,
                       void* start,
                       Metadata* endType,
                       void* end);

  template<typename T, typename U>
  const typename T GetFirst(const typename std::unordered_map<T, U>::iterator& it)
  {
    return it->first;
  }

  template<typename T, typename U>
  U& GetSecond(const typename std::unordered_map<T, U>::iterator& it)
  {
    return it->second;
  }

  //template<typename T, typename U>
  //U& GetContent(const typename T<typename U>::iterator& it)
  //{
  //  return *it;
  //}

  template<typename T, typename U>
  bool FillContainer(T& container, std::string directory, std::string extension)
  {
    std::vector<std::string> filenames(FindFilesByExtension(directory, extension));
    for (auto& it : filenames)
    {
      container.push_back(new U());
      if (FindType(U)->GetMethods().find("Load") != FindType(U)->GetMethods().end())
      {
        Var selfRef(*container.back());
        bool retval = false;
        Var RetVal(retval);
        Var filename(it);
        std::vector<Var*> args;
        args.push_back(&filename);
        FindType(U)->GetMethods()["Load"](&selfRef, &RetVal, args);
        if (!retval)
        {
          return retval;
        }
      }
      else
      {
        if (!container.back()->Deserialize(it))
        {
          return false;
        }
      }
    }
    return true;
  }

  template<typename U>
  bool FillContainer(std::unordered_map<std::string, U*>& container, std::string directory, std::string extension)
  {
    std::vector<std::string> filenames(FindFilesByExtension(directory, extension));
    for (auto& it : filenames)
    {
      unsigned start = it.find_last_of('/');
      unsigned end = it.find_last_of('.') - start;
      std::string name = it.substr(start, end);
      container[name] = new U();
      if (FindType(U)->GetMethods().find("Load") != FindType(U)->GetMethods().end())
      {
        Var selfRef(*container[name]);
        bool retval = false;
        Var RetVal(retval);
        Var filename(it);
        std::vector<Var*> args;
        args.push_back(&filename);
        FindType(U)->GetMethods()["Load"](&selfRef, &RetVal, args);
        if (!retval)
        {
          return retval;
        }
      }
      else
      {
        if (!container[name]->Deserialize(it))
        {
          return false;
        }
      }
    }
    return true;
  }

  template<typename T>
  void ConstructType(void* object)
  {
    new (object) T;
  }

  template<typename T, typename argType1>
  void ConstructType(void* object, argType1 arg1)
  {
    new (object) T(arg1);
  }

  template<typename T, typename argType1, typename argType2>
  void ConstructType(void* object, argType1 arg1, argType2 arg2)
  {
    new (object) T(arg1, arg2);
  }

  template<typename T, typename argType1, typename argType2, typename argType3>
  void ConstructType(void* object, argType1 arg1, argType2 arg2, argType3 arg3)
  {
    new (object) T(arg1, arg2, arg3);
  }

  template<typename T, typename argType1, typename argType2, typename argType3, typename argType4>
  void ConstructType(void* object, argType1 arg1, argType2 arg2, argType3 arg3, argType4 arg4)
  {
    new (object) T(arg1, arg2, arg3, arg4);
  }

  template<typename T, typename argType1, typename argType2, typename argType3, typename argType4, typename argType5>
  void ConstructType(void* object, argType1 arg1, argType2 arg2, argType3 arg3, argType4 arg4, argType5 arg5)
  {
    new (object) T(arg1, arg2, arg3, arg4, arg5);
  }

  template<typename T>
  void DestructType(void* object)
  {
    (static_cast<T*>(object))->T::~T();
  }
}
