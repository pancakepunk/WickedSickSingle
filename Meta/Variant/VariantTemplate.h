#pragma once
#include "meta/Utility/MetaMacros.h"
#include "meta/Type/Metadata.h"
#include "meta/Utility/IndirectionCount.h"

namespace Reflection
{


  template<typename T>
  Var::Var(const T& data) : is_valid_(true),
    levels_of_indirection_(IndirectionCount<T>::value),
    owns_(false)
  {
    data_ = reinterpret_cast<void*>(const_cast<T*>(&data));
    type_ = META(data);

  }

  template<typename T>
  Var::Var(T& data) : is_valid_(true),
    levels_of_indirection_(IndirectionCount<T>::value),
    owns_(false)
  {
    data_ = reinterpret_cast<void*>(&data);
    type_ = META(data);

  }

  template<typename T>
  T& Var::GetValue(void)
  {
    return *reinterpret_cast<T*>(data_);
  }


  template<typename T>
  void Var::SetValue(const T& value)
  {
    T* constAway = const_cast<T*>(&value);
    SetValue<T>(*constAway);
  }

  template<typename T>
  void Var::SetValue(T& value)
  {
    void* target = data_;
    for (int i = 0; i < levels_of_indirection_; ++i)
    {
      target = *reinterpret_cast<void**>(target);
    }
    memcpy(target, &value, sizeof(T));
  }

  template<>
  void Var::SetValue(const char*& value)
  {
    void* target = data_;
    for (int i = 1; i < levels_of_indirection_; ++i)
    {
      target = *reinterpret_cast<void**>(target);
    }
    strncpy((char*)target, value, strlen(value));
    __debugbreak();
  }

  template<>
  void Var::SetValue(void*& value)
  {
    void* target = data_;
    //we want a pointer to our data, not the data itself
    for (int i = 1; i < levels_of_indirection_; ++i)
    {
      target = *reinterpret_cast<void**>(target);
    }
    memcpy(target, value, type_->GetSize());
    __debugbreak();
  }

  template<>
  void Var::SetValue(std::string& value)
  {
    if (levels_of_indirection_ != 0)
    {
      __debugbreak();
    }
    new (data_) std::string(value);
  }



  template <typename Type>
  void Copy(const Type& type)
  {
    Metadata* metatype = FindType(Type);
    if (metatype)
    {

    }
  }


  template<typename T>
  Var::Var(T&& data) : is_valid_(true),
    levels_of_indirection_(IndirectionCount<T>::value)
  {
    unsigned short size = sizeof(T);
    if (FindType(T) == FindType(char))
    {
      size = MAX_CSTRING;
    }
    data_ = new char[size];
    memcpy(data_, &data, size);
    type_ = FindType(T);
    if (type_ == FindType(std::string))
    {
      //reinterpret_cast<std::string*>(data_)->get_buf() = new char [reinterpret_cast<std::string*>(data_)->capacity()]; //why are we doing this
      reinterpret_cast<std::string*>(data_)->assign(*reinterpret_cast<std::string*>(const_cast<void*>(reinterpret_cast<const void*>(&data))));
    }
  }

  template<>
  Var::Var(LuaUserdata * const& tocopy) : levels_of_indirection_(0),
    is_valid_(true)
  {
    data_ = new char[sizeof(LuaUserdata)];
    memcpy(data_, tocopy, sizeof(LuaUserdata));
  }

  template<>
  Var::Var(void * const& tocopy) : levels_of_indirection_(0),
    is_valid_(true),
    data_(tocopy)
  {
    data_ = new char[sizeof(LuaUserdata)];
    memcpy(data_, tocopy, sizeof(LuaUserdata));
  }
}
