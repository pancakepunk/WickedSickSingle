#pragma once

namespace WickedSick
{
  enum FailReason
  {
    #define RegisterReason(x) x,
    #include "FailReasons.h"
    #undef RegisterReason
  };

  struct TestInfo
  {
    float time;
    int bytesAllocated;
    int bytesLeaked;
    bool success;
    FailReason reason;
  };
}
