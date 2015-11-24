#pragma once
#include "Math/MathInterface.h"
#include "API/APIType.h"
#include "General/GraphicsTypes.h"

namespace WickedSick
{
  struct GraphicsOptions
  {
    GraphicsOptions(bool     vSync = false,
                    const Vector2i& resolution = Vector2i(1200, 720),
                    const Vector4&  clearColor = Vector4(0.0f, 0.0f, 1.0f, 1.0f),
                    APIType::Enum  api = APIType::DirectX);


    bool            VSync;
    Vector2i        Resolution;
    Vector4         ClearColor;
    APIType::Enum         Api;

    //modifiable stuff goes here i.e.
    //int           shadowDetail;
    //int           antiAliasing;
    //int           textureDetail;
    //int           reflectionDetail;
    //bool          bloom;
    //LightingType  lighting;
  };
}