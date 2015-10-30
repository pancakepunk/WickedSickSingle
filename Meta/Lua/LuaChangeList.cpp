
//
#include "meta/Lua/LuaChangeList.h"
#include "meta/Type/Member.h"
#include "meta/Type/Metadata.h"
#include "meta/Lua/LuaUserdata.h"

namespace Reflection
{
  LuaChange::LuaChange() : member(nullptr)
  {

  }

  LuaChange::LuaChange(LuaMemberdata* mem, char* ch) : member(member)
  {
    memcpy(data, ch, mem->meta->GetSize());
  }

  void LuaChangeList::CommitChange(LuaMemberdata* member, char* change)
  {
    if(changes_.find(member) == changes_.end())
    {
      changes_[member] = LuaChange(member, change);
    }
    else
    {
      std::cout << "two changes committed to same member on object" << std::endl;//get Mark.
      __debugbreak();
    }
  }

  void LuaChangeList::ForceChange(LuaMemberdata* member, char* change)
  {
    changes_[member] = LuaChange(member, change);
  }

  void LuaChangeList::PushChanges()
  {
    for(auto& it : changes_)
    {
      memcpy((char*)target_ + it.first->offset, 
             (char*)it.second.data + it.first->offset, 
             it.first->meta->GetSize());
    }
    changes_.clear();
  }
}