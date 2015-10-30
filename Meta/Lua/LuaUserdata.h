#pragma once
#include <string>
namespace Reflection
{
  class Metadata;
  class LuaUserdata
  {
    public:
      void SetMember(std::string name, void* value);

      void SetObject(Metadata* type, void* value);

      void SetType(Metadata* type);

      void SetIndirection(unsigned ind);

      unsigned GetIndirection();

      char* GetData();

      Metadata* GetType()
      {
        return type_;
      }
    private:
      Metadata* type_;
      unsigned indirection_;
  };

  struct LuaMemberdata
  {
    enum CBType
    {
      Get,
      Set
    };
    unsigned indirection;
    const char* name;
    Metadata* meta;
    unsigned offset;
    CBType type;
  };
}