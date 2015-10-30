#pragma once



namespace WickedSick
{
  class Buffer;
  struct Renderable
  {
    Renderable();
    Buffer* indexBuf;
    Buffer* vertBuf;
  };
}
