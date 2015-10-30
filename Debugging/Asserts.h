#pragma once

#ifdef _DEBUG
  #define WSAssert(condition, errorMessage) \
  if (!(condition))                           \
  {                                         \
    ConsolePrint(errorMessage);             \
    __debugbreak();                         \
    throw(0);                               \
  }
#endif

#define WSError(errorMessage)             \
  ConsolePrint(errorMessage);             \
  __debugbreak();                         \
  throw(0);                               \

#ifdef _RELEASE
  #define WSAssert(condition, errorMessage) \
  if (!(condition))                           \
  {                                         \
    ConsolePrint(errorMessage);             \
  }
#endif
