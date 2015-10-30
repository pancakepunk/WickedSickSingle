#pragma once

#include "meta/Type/Metadata.h"

namespace Reflection
{
  template <typename MetaType>
  class MetaSingleton
  {
  public:
    MetaSingleton()
    {
      RegisterTypeData();
    }
    static Metadata* Get()
    {
      return &metadata_;
    }

    template<typename baseType>
    class Derived
    {
    public:
      static Metadata* Get() { return metadata_; }
      static Metadata* metadata_;
    };

    static void RegisterTypeData(void);
    static Metadata metadata_;
  private:
  };

  template <typename MetaType>
  class MetaSingleton<const MetaType> : public MetaSingleton<MetaType> {};

  template <typename MetaType>
  class MetaSingleton<MetaType&> : public MetaSingleton<MetaType> {};

  template <typename MetaType>
  class MetaSingleton<const MetaType&> : public MetaSingleton<MetaType> {};

  template <typename MetaType>
  class MetaSingleton<MetaType&&> : public MetaSingleton<MetaType> {};

  template <typename MetaType>
  class MetaSingleton<MetaType*> : public MetaSingleton<MetaType> {};

  template <typename MetaType>
  class MetaSingleton<const MetaType*> : public MetaSingleton<MetaType> {};

}
