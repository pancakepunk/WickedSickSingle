#include "Precompiled.h"
//
#include "meta/Utility/MetaUtility.h"
#include "meta/Utility/MetaMacros.h"
#include "meta/Type/Metadata.h"
#include "meta/Type/MetaSingleton.h"
namespace Reflection
{

  uint32_t SuperFastHash(const char * data, int len)
  {
    uint32_t hash = len, tmp;
    int rem;

    if(len <= 0 || data == nullptr) return 0;

    rem = len & 3;
    len >>= 2;

    /* Main loop */
    for(; len > 0; len--)
    {
      hash += get16bits(data);
      tmp = (get16bits(data + 2) << 11) ^ hash;
      hash = (hash << 16) ^ tmp;
      data += 2 * sizeof(uint16_t);
      hash += hash >> 11;
    }

    /* Handle end cases */
    switch(rem)
    {
      case 3: hash += get16bits(data);
        hash ^= hash << 16;
        hash ^= ((signed char) data[sizeof(uint16_t)]) << 18;
        hash += hash >> 11;
        break;
      case 2: hash += get16bits(data);
        hash ^= hash << 11;
        hash += hash >> 17;
        break;
      case 1: hash += (signed char) *data;
        hash ^= hash << 10;
        hash += hash >> 1;
    }

    /* Force "avalanching" of final 127 bits */
    hash ^= hash << 3;
    hash += hash >> 5;
    hash ^= hash << 4;
    hash += hash >> 17;
    hash ^= hash << 25;
    hash += hash >> 6;

    return hash;
  }


  void GenericSetValue(Metadata* startType, void* start, Metadata* endType, void* end)
  {
    if (startType->GetEnumConstants().size())
    {
      startType = FindType(int);
    }
    if (endType->GetEnumConstants().size())
    {
      endType = FindType(int);
    }

    if (endType == startType)
    {
      memcpy(end, start, startType->GetSize());
    }

    else if (startType == FindType(char))
    {
      if (endType == FindType(std::string))
      {
        if (start)
        {
          new (end) std::string((char*)start);
        }
        else
        {
          new (end) std::string("");
        }
      }
    }
    else if (startType == FindType(std::string))
    {
      if (endType == FindType(char))
      {
        if (start)
        {
          const char* temp = reinterpret_cast<std::string*>(start)->c_str();
          memcpy(end, temp, reinterpret_cast<std::string*>(start)->size() + 1);
        }
        else
        {
          new (end) std::string("");
        }
      }
    }
    else if (startType == FindType(bool))
    {
      if (endType == FindType(bool))
      {
        new (end) bool(*reinterpret_cast<bool*>(start));
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<bool*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<bool*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<bool*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<bool*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<bool*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<bool*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<bool*>(start));
        new (end) unsigned(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(char))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<char*>(start) != 0);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        new (end) char(*reinterpret_cast<char*>(start));
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<char*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<char*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<char*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<char*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<char*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<char*>(start));
        new (end) unsigned(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(short))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<short*>(start) != 0);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<short*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        new (end) short(*reinterpret_cast<short*>(start));
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<short*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<short*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<short*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<short*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<short*>(start));
        new (end) unsigned(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(int))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<int*>(start) != 0);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<int*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<int*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        new (end) int(*reinterpret_cast<long*>(start));
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<int*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<int*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<int*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<int*>(start));
        new (end) unsigned(endtype);
      }
      else if (endType == FindType(uint16_t))
      {
        uint16_t endtype = static_cast<uint16_t>(*reinterpret_cast<int*>(start));
        new (end) uint16_t(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(long))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<long*>(start) != 0l);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<long*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<long*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<long*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        new (end) long(*reinterpret_cast<long*>(start));
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<long*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<long*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<long*>(start));
        new (end) unsigned(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(float))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<float*>(start) != 0.0f);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<float*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<float*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<float*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<float*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        new (end) float(*reinterpret_cast<float*>(start));
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<float*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<float*>(start));
        new (end) unsigned(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(double))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<double*>(start) != 0.0);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<double*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<double*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<double*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<double*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<double*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        new (end) double(*reinterpret_cast<double*>(start));
      }
      else if (endType == FindType(unsigned))
      {
        unsigned endtype = static_cast<unsigned>(*reinterpret_cast<double*>(start));
        new (end) unsigned(endtype);
      }
      else if (endType == FindType(uint16_t))
      {
        uint16_t endtype = static_cast<uint16_t>(*reinterpret_cast<double*>(start));
        new (end) uint16_t(endtype);
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(unsigned))
    {
      if (endType == FindType(bool))
      {
        bool endtype = (*reinterpret_cast<unsigned*>(start) != 0);
        new (end) bool(endtype);
      }
      else if (endType == FindType(char))
      {
        char endtype = static_cast<char>(*reinterpret_cast<unsigned*>(start));
        new (end) char(endtype);
      }
      else if (endType == FindType(short))
      {
        short endtype = static_cast<short>(*reinterpret_cast<unsigned*>(start));
        new (end) short(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<unsigned*>(start));
        new (end) int(endtype);
      }
      else if (endType == FindType(long))
      {
        long endtype = static_cast<long>(*reinterpret_cast<unsigned*>(start));
        new (end) long(endtype);
      }
      else if (endType == FindType(float))
      {
        float endtype = static_cast<float>(*reinterpret_cast<unsigned*>(start));
        new (end) float(endtype);
      }
      else if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<unsigned*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(unsigned))
      {
        new (end) unsigned(*reinterpret_cast<unsigned*>(start));
      }
      else
      {
        __debugbreak();
        //PopError("Meta Error", "Type not defined in GenericSetValue.");
      }
    }
    else if (startType == FindType(uint16_t))
    {
      if (endType == FindType(double))
      {
        double endtype = static_cast<double>(*reinterpret_cast<uint16_t*>(start));
        new (end) double(endtype);
      }
      else if (endType == FindType(int))
      {
        int endtype = static_cast<int>(*reinterpret_cast<uint16_t*>(start));
        new (end) int(endtype);
      }
    }
    else
    {
      __debugbreak();
      //PopError("Meta Error", "Type not defined in GenericSetValue.");
    }
  }

  void StringToVal(std::string input, Metadata* outType, void* out)
  {

    if (FindType(int) == outType)
    {
      *reinterpret_cast<int*>(out) = std::atoi(input.c_str());
    }
    else if (FindType(std::string) == outType)
    {
      *reinterpret_cast<std::string*>(out) = input;
    }
    else if (FindType(double) == outType)
    {
      *reinterpret_cast<double*>(out) = std::atof(input.c_str());
    }
    else if (FindType(float) == outType)
    {
      *reinterpret_cast<float*>(out) = static_cast<float>(std::atof(input.c_str()));
    }
    else if (FindType(long) == outType)
    {
      *reinterpret_cast<long*>(out) = std::atol(input.c_str());
    }
    else if (FindType(bool) == outType)
    {
      if (input == "true")
      {
        *reinterpret_cast<bool*>(out) = true;
      }
      else if (input == "false")
      {
        *reinterpret_cast<bool*>(out) = false;
      }
      else
      {
        __debugbreak();
        //Print("Wrong input for boolean value.\n");
      }

    }
    else
    {
      __debugbreak();
      //Print("Type doesn't match any listed type. Bound the wrong thing to a field?\n");
    }
  }

  std::string GetStringOf(Metadata* outType, void* out)
  {
    if (!outType->GetEnumConstants().empty())
    {
      outType = FindType(int);
    }
    if (FindType(int) == outType)
    {
      return std::to_string(*reinterpret_cast<int*>(out));
    }
    else if (FindType(std::string) == outType)
    {
      return *reinterpret_cast<std::string*>(out);
    }
    else if (FindType(double) == outType)
    {
      return std::to_string(*reinterpret_cast<double*>(out));
    }
    else if (FindType(float) == outType)
    {
      return std::to_string(*reinterpret_cast<float*>(out));
    }
    else if (FindType(long) == outType)
    {
      return std::to_string(*reinterpret_cast<long*>(out));
    }
    else if (FindType(bool) == outType)
    {
      if (*reinterpret_cast<bool*>(out))
      {
        return "true";
      }
      return "false";
    }
    else
    {
      __debugbreak();
      //Print("Type doesn't match any listed type. Bound the wrong thing to a field?\n");
    }
    return "NO_TYPE";
  }

  void* resolveIndirection(int from, int to, void * fromPtr, void * toPtr)
  {
    //this is levels of indirection
    //we at most only want pointers to whatever our things are
    //so we may -1 from each from and to
    --from;
    --to;
    if (from > to)
    {
      while (from > to)
      {
        fromPtr = *reinterpret_cast<void**>(fromPtr);
        --from;
      }
      return fromPtr;
    }
    else if (to > from)
    {
      while (to > from + 1)
      {
        toPtr = *reinterpret_cast<void**>(toPtr);
        --from;
      }
      *reinterpret_cast<void**>(toPtr) = fromPtr;
      return toPtr;
    }
    else
    {
      return fromPtr;
    }


    return nullptr;
  }

  unsigned int MetaHash(Metadata* meta, void* object)
  {
    unsigned initial = SuperFastHash((char*)object, meta->GetSize());
    for (auto& it : meta->GetMembers())
    {
      if (it.second.GetType() == FindType(std::string))
      {
        std::string* string = (std::string*)((char*)object + it.second.GetOffset());
        initial += SuperFastHash(string->c_str(), string->size());
      }
    }
    return initial;
  }
}
