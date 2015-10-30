#pragma once
#include "ParamTypeInfo.h"

namespace WickedSick
{
  class Graphics;
  class ModelComponent;
  class Shader;
  
  struct EmitterDescription;
  struct ParticleDescription;
  struct LightingShaderParam
  {
    ModelComponent* modelComp;
    Shader* shader;
  };
  struct ParticleShaderParam
  {
    ParticleDescription* particle;
    EmitterDescription* emitter;
    Shader* shader;
  };
  void ParticleShaderCallback(Graphics* gSys, void* data, std::vector<ParamPasser>& params);
  void LineShaderCallback(Graphics* gSys, void* data, std::vector<ParamPasser>& params);
  void vertexShaderCallback(Graphics* gSys, void* comp, std::vector<ParamPasser>& params);
  void WireframeShaderCallback(Graphics* gSys, ModelComponent* comp, std::vector<ParamPasser>& params);
}