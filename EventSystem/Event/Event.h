#pragma once

#include "Core/CoreInterface.h"
#include "Utility/UtilityInterface.h"
#include "meta/MetaInterface.h"


namespace WickedSick
{

  class EventSystem;

  class Event
  {
  public:
    
    Event( const std::string& name);

    std::string GetName();

    //kinda need my meta back....
    template<typename Param>
    void Add(const std::string& id, const Param& toAdd)
    {
      
      variables_.insert(id, new Reflection::Var(toAdd));
    }

    template<typename Param>
    Param* Get(const std::string& id)
    {
      auto& it = variables_.find(id);
      if(it != variables_.end())
      {
        Reflection::Var* variable = ((*it).val);
        return &variable->GetValue<Param>();
      }
      return nullptr;
    }


    //template<typename T>
    //void Set(const std::string& name, const T& val)
    //{
    //  auto& it = variables_.find(name);
    //  if (it != variables_.end())
    //  {
    //    (*it).val->Set(val);
    //  }
    //  else
    //  {
    //    variables_.insert(name, Var(val));
    //  }
    //}
    //
    //Var* Get(const std::string& name)
    //{
    //  auto& it = variables_.find(name);
    //  if (it != variables_.end())
    //  {
    //    return &((*it).val);
    //  }
    //  return nullptr;
    //}



    virtual ~Event();


  private:

    HashMap<std::string, Reflection::Var*> variables_;

    std::string       event_name_;
    int               event_ID_;

    static int        id_count_;

  protected:

    static EventSystem* event_system_;

    

  };
}
