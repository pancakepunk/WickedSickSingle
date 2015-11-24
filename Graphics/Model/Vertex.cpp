#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "Vertex.h"

#include "Debugging/DebugInterface.h"

namespace WickedSick
{
  bool Vertex::operator==(const Vertex& rhs)
  {
    return (position == rhs.position) && (normal == rhs.normal) && (tangent == rhs.tangent) && (bitangent == rhs.bitangent) && (tex == rhs.tex);
  }
  std::string Vertex::to_string()
  {
    return position.to_string() + normal.to_string() + tangent.to_string() + bitangent.to_string() + tex.to_string();
  }
  void Vertex::Print()
  {
    ConsolePrint("\n position: ");
    position.Print();

    // ConsolePrint("\n normal: ");
    // normal.Print();
    //ConsolePrint("\n color: ");
    //color.Print();
  }
}
