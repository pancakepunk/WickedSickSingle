#include "Precompiled.h"

//
#include "meta/Utility/MetaMacros.h"
#include "meta/Type/MetaSingleton.h"
#include "meta/Type/MetaManager.h"
#include "meta/Type/Metadata.h"
#include "meta/Definitions/Primitives.h"
#include "meta/Definitions/ExternalTypes.h"




DEFINE_PRIMITIVE(bool)

DEFINE_PRIMITIVE(uint16_t)

DEFINE_PRIMITIVE(short)

DEFINE_PRIMITIVE(int)

DEFINE_PRIMITIVE(long)

DEFINE_PRIMITIVE(float)

DEFINE_PRIMITIVE(double)

DEFINE_PRIMITIVE(unsigned)

DEFINE_PRIMITIVE(char)

//void
Reflection::Metadata Reflection::MetaSingleton<void>::metadata_("void", 0, ByValue, true);
Reflection::MetaSingleton<void> voidMetaSingleton;
template <> void Reflection::MetaSingleton<void>::RegisterTypeData(void)
{
}

DEFINE_CONVERSIONS
{
  
  CONVERTS(char, std::string);
  CONVERTS(std::string, char);
  CONVERTS(char, unsigned);
  CONVERTS(char, double);
  CONVERTS(char, float);  
  CONVERTS(char, int);
  CONVERTS(char, short);
  CONVERTS(char, long);
  CONVERTS(char, bool);

  CONVERTS(unsigned, double);
  CONVERTS(unsigned, float);  
  CONVERTS(unsigned, int);
  CONVERTS(unsigned, short);
  CONVERTS(unsigned, long);
  CONVERTS(unsigned, bool);

  CONVERTS(double, float);
  CONVERTS(double, int);
  CONVERTS(double, short);
  CONVERTS(double, long);
  CONVERTS(double, bool);

  CONVERTS(float, int);
  CONVERTS(float, short);
  CONVERTS(float, bool);
  CONVERTS(float, long);

  CONVERTS(long, int);
  CONVERTS(long, short);
  CONVERTS(long, bool);

  CONVERTS(int, bool);
  CONVERTS(int, short);
  CONVERTS(uint16_t, double);
  CONVERTS(uint16_t, int);

  CONVERTS(double, uint16_t);
  CONVERTS(int, uint16_t);

  CONVERTS(short, bool);
}