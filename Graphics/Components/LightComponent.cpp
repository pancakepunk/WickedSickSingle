#include "GraphicsPrecompiled.h"
#include "LightComponent.h"
#include "System/Graphics.h"

namespace WickedSick
{
  LightComponent::LightComponent() : Component(CT_LightComponent)
  {

  }

  LightComponent::~LightComponent()
  {

  }

  void LightComponent::Initialize()
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    gSys->AddLight(this);
  }

  void LightComponent::Update(float dt)
  {
    Transform* tr = (Transform*)GetSibling(CT_Transform);
    Graphics* gSys = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    Vector3 curPos = tr->GetPosition();
    if(gSys->GetSceneConstants().rotate)
    {
      //have to update this shit because it says so.
      
      tr->SetPosition(RotateAround(Vector3(0.0f, 1.0f, 0.0f), dt / 10.0f, curPos));
    }


    curPos = tr->GetPosition();
    switch(light_type_)
    {
      case WickedSick::LightType::SpotLight:
        if(curPos != info_.spot.position)
        {
          Dirty();
          info_.spot.position = curPos;
        }
        break;
      case WickedSick::LightType::PointLight:
        if(curPos != info_.point.position)
        {
          Dirty();
          info_.point.position = curPos;
        }
        break;
      default:
        break;
    }
  }


  LightInfo& LightComponent::GetInfo()
  {
    return info_;
  }

  size_t LightComponent::GetSize()
  {
    size_t toReturn = 0;
    switch(light_type_)
    {
      case LightType::DirectionalLight:
      {
        toReturn += sizeof(info_.dir);
        break;
      }
      case LightType::PointLight:
      {
        toReturn += sizeof(info_.point);
        break;
      }
      case LightType::SpotLight:
      {
        toReturn += sizeof(info_.spot);
        break;
      }
    }
    return toReturn;
    
  }

  LightType::Enum LightComponent::GetLightType()
  {
    return light_type_;
  }

  void LightComponent::SetLightType(LightType::Enum type)
  {
    light_type_ = type;
  }

  void LightComponent::Dirty()
  {
    Graphics* gSys = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    gSys->DirtyLights();
  }

  LightInfo::LightInfo()
  {
    new (&spot) Spot();
  }

  LightInfo::LightInfo(const LightInfo& rhs)
  {
    memcpy(this, &rhs, sizeof(LightInfo));
  }

  LightInfo::LightInfo(LightInfo && rhs)
  {
    memcpy(this, &rhs, sizeof(LightInfo));
  }


  Dir::Dir() 
  : ambientIntensity(0.2f, 0.2f, 0.2f),
    diffuseIntensity(0.2f, 0.2f, 0.2f),
    specularIntensity(0.2f, 0.2f, 0.2f)
  {
  }


  Spot::Spot()
  : ambientIntensity(0.2f, 0.2f, 0.2f),
    diffuseIntensity(0.2f, 0.2f, 0.2f),
    specularIntensity(0.2f, 0.2f, 0.2f),
    phi(15.0f * PI / 180.0f),
    theta(30.0f * PI / 180.0f),
    angleFalloff(0.5f),
    pad0(0.0f, 0.0f, 0.0f),
    attenuationConstants(1.0f, 0.1f, 0.0f),
    position(0.0f, 0.0f, 0.0f),
    direction(0.0f, -1.0f, 0.0f)
  {
  }


  Point::Point()
  : ambientIntensity(0.2f, 0.2f, 0.2f),
    diffuseIntensity(0.2f, 0.2f, 0.2f),
    specularIntensity(0.2f, 0.2f, 0.2f),
    attenuationConstants(1.0f, 0.1f, 0.0f),
    position(0.0f, 0.0f, 0.0f),
    pad0(0.0f)
  {
    memset(this, 0, sizeof(Point));
  }
}


RegisterType(WickedSick, Point)
RegisterMember(attenuationConstants);
RegisterMember(ambientIntensity);
RegisterMember(diffuseIntensity);
RegisterMember(specularIntensity);
}

RegisterType(WickedSick, Spot)
RegisterMember(attenuationConstants);
RegisterMember(ambientIntensity);
RegisterMember(diffuseIntensity);
RegisterMember(specularIntensity);
RegisterMember(direction);
RegisterMember(phi);
RegisterMember(theta);
RegisterMember(angleFalloff);
}

RegisterType(WickedSick, Dir)
RegisterMember(ambientIntensity);
RegisterMember(diffuseIntensity);
RegisterMember(specularIntensity);
RegisterMember(direction);
}

RegisterType(WickedSick, LightInfo)
RegisterMember(point);
RegisterMember(spot);
RegisterMember(dir);
}


RegisterType(WickedSick, LightComponent)
RegisterMember(info_);
RegisterMember(light_type_);
}