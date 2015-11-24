#pragma once

#include "meta/Type/Member.h"
#include "meta/Utility/MetaMacros.h"
#include "meta/Constructor/Constructor.h"
#include "meta/Lua/LuaUserdata.h"
#include "meta/Lua/LuaIncludes.h"


namespace Reflection
{

  class Function;
  class MetaManager;
  enum ValueType
  {
    ByReference,
    ByValue,
    ByPointer, //not sure if this is useful at all
    ByNone
  };


  enum Language
  {
    FromLua,
    FromCPlusPlus,
    FromNone
  };
  class Metadata
  {
  public:
    MetaDef;

    Metadata(const std::string& name,
             const std::string& Namespace,
             size_t size,
             ValueType valType = ByValue,
             bool built_in_type = false);

    Metadata(const std::string& name,
             size_t size,
             ValueType valType = ByValue,
             bool built_in_type = false);

    void AddMember(const Member& member);
    void AddMethod(Function* function, const std::string& name);
    void AddConstructor(const std::string& name, const Constructor& ctor);
    void AddDerived(Metadata* type);

    std::string GetName() const;
    std::string GetNamespace() const;

    const size_t& GetSize() const;

    ValueType GetValueType() const;

    Member* GetMember(const std::string& name);
    Function* GetMethod(const std::string& name);
    Constructor* GetConstructor(const std::string& name);

    std::set<Metadata*>& GetDerived();
    std::map<std::string, Member>& GetMembers();
    std::unordered_map<std::string, Function*>& GetMethods();
    std::unordered_map<std::string, Constructor>& GetConstructors();

    std::vector<const char*>& GetEnumConstants();

    void AddConversion(Metadata* type);
    bool Converts(Metadata* type);

    bool IsBuiltInType();

    std::vector<luaL_Reg>& GetLuaMethods();
    std::vector<luaL_Reg>& GetLuaMetaMethods();

    std::vector<LuaMemberdata>& GetLuaGetters();
    std::vector<LuaMemberdata>& GetLuaSetters();


    bool InheritsFrom(Metadata* type);
    bool BaseClassOf(Metadata* type);

  private:
    std::string name_;
    std::string namespace_;

    ValueType   val_type_;

    Metadata* inherits_from_;
    std::set<Metadata*> derived_;

    std::vector<luaL_Reg> lua_method_metatable_;
    std::vector<luaL_Reg> lua_metamethod_metatable_;
    std::vector<LuaMemberdata> lua_getter_metatable_;
    std::vector<LuaMemberdata> lua_setter_metatable_;

    std::unordered_map<std::string, Metadata*> conversions_;
    std::map<std::string, Member> members_;
    std::unordered_map<std::string, Function*> methods_;
    std::unordered_map<std::string, Constructor> constructors_;



    size_t size_;
    bool built_in_type_;
    std::vector<const char*> enum_constants_;
  };
}
