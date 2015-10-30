#pragma once
#include <unordered_map>
#include <string>
namespace Reflection
{
  class Script;
  class ScriptManager
  {
    public:
      ScriptManager();
      ~ScriptManager();
      void LoadScripts();
      void ReloadScripts();
      Script* MakeScript(const std::string& file);
      Script* GetScript(const std::string& name);
    private:
      std::unordered_map<std::string, Script*> scripts_;
  };
}
