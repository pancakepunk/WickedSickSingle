#pragma once
#include "Math/MathInterface.h"

namespace WickedSick
{
  namespace InputShaderType
  {
    enum Enum
    {
      Pixel,
      Vertex,
      Geo,
      Tesselation,
      Compute
    };
  };

  namespace DataType
  {
    enum Enum
    {
      Int,
      Unsigned,
      Float,
      Typeless,
      Custom
    };
  };

  struct ParamPasser
  {
    ParamPasser(const std::string& buffer,
                const std::string& name,
                const void* data,
                int size);
    ParamPasser(const ParamPasser& toCopy);
    ParamPasser(ParamPasser&& moveFrom);
    void Release();
    ~ParamPasser();
    std::string Buffer;
    std::string Name;
    char Data[900];
    int Size;
  };

  struct ParamTypeInfo
  {
    std::string name;
    size_t size;
    size_t offset;
  };

}
