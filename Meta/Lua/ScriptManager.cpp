#include "Precompiled.h"

#include "meta/Lua/ScriptManager.h"
#include "meta/Lua/Script.h"

namespace Reflection
{
  ScriptManager::ScriptManager()
  {
    
  }

  ScriptManager::~ScriptManager()
  {
    for(auto it : scripts_)
    {
      delete it.second;
    }
  }

  void ScriptManager::LoadScripts()
  {
    //need to implement that...
    std::vector<std::string> files;// = FilesByExtensionRecursive("Scripts", "lua");
    for(auto& it : files)
    {
      MakeScript(it);
    }
    for(auto it : scripts_)
    {
      it.second->Load();
    }
  }

  void ScriptManager::ReloadScripts()
  {
    for(auto it : scripts_)
    {
      it.second->Unload();
      it.second->Load();
    }
  }

  Script* ScriptManager::MakeScript(const std::string& file)
  {
    if(file.size())
    {
      scripts_[file] = new Script(file);
      return scripts_[file];
    }
    return nullptr;
  }

  Script* ScriptManager::GetScript(const std::string& name)
  {
    std::string standardSlashes = name;
    unsigned index = standardSlashes.find_first_of("\\");
    while (index != std::string::npos) 
    {
      standardSlashes[index] = '/';
      index = standardSlashes.find_first_of("\\");
    }
    

    auto it = scripts_.find(standardSlashes);
    if(it != scripts_.end())
    {
      return it->second;
    }
    return NULL;
  }

}
