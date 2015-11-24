#include "Precompiled.h"
///////////////////////////////////////////////////////////////////////////
//Author:      Mark
//Date:        1/21/2014
//Description: holds function to register metadata
//All content (c) 2014 DigiPen (USA) Corporation, all rights reserved.
///////////////////////////////////////////////////////////////////////////
//
#include "meta/Type/MetaManager.h"
#include "meta/Type/Metadata.h"

namespace Reflection
{
  void MetaManager::RegisterMeta(Metadata* meta)
  {
    getMetas()[meta->GetName()] = meta;
  }
}

