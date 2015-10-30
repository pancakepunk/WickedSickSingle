#pragma once


#include "meta/Utility/SimplifyType.h"
#include "meta/Type/MetaSingleton.h"

namespace Reflection
{

  class Metadata;
  class MetaManager
  {
  public:
    static void RegisterMeta(Metadata* meta);

    static Metadata* Get(const std::string& name)
    {
      MetaMap& metas = getMetas();
      auto meta = metas.find(name);
      if (meta == metas.end())
      {
        return nullptr;
      }
      else
      {
        return meta->second;
      }
    }

    static void DefineConversions();

    typedef std::unordered_map<std::string, Metadata*> MetaMap;

    static MetaMap& getMetas()
    {
      static MetaMap metas;
      return metas;
    }
  };
}
