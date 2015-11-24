#pragma once

#include "Math/MathInterface.h"
#include "Core/CoreInterface.h"

#include "meta/MetaInterface.h"
#include "General/GraphicsTypes.h"
namespace WickedSick
{
  struct Point
  {
    MetaDef;
    Point();
    Vector3 attenuationConstants;
    float dumb;
    Vector3 position;
    float i;
    Vector3 ambientIntensity;
    float hate;
    Vector3 diffuseIntensity;
    float everything;
    Vector3 specularIntensity;
    float pad0;
    //48bytes
  };

  struct Spot
  {
    MetaDef;
    Spot();
    Vector3 attenuationConstants;
    Vector3 position;

    Vector3 ambientIntensity;
    Vector3 diffuseIntensity;
    Vector3 specularIntensity;

    Vector3 direction;

    float phi;
    float theta;
    float angleFalloff;

    Vector3 pad0;
    //64bytes
  };
  struct Dir
  {
    MetaDef;
    Dir();
    Vector3 ambientIntensity;
    float pad0;
    Vector3 diffuseIntensity;
    float pad1;
    Vector3 specularIntensity;
    float pad2;
    Vector3 direction;
    //32bytes
  };
  //alligned for graphics dont fuck with it please
  struct LightInfo
  {
    MetaDef;
    LightInfo();
    LightInfo(const LightInfo& rhs);
    LightInfo(LightInfo&& rhs);


    Point point;
    Spot spot;
    Dir dir;

  };

  class LightComponent : public Component
  {
  public:

    MetaDef;

    LightComponent();
    ~LightComponent();

    void Initialize() override;
    void Update(float dt) override;
    LightInfo& GetInfo();
    size_t GetSize();

    void Dirty();
    LightType::Enum GetLightType();
    void SetLightType(LightType::Enum type);

  private:
    bool dirty_;
    LightType::Enum light_type_;
    LightInfo info_;

  };
}