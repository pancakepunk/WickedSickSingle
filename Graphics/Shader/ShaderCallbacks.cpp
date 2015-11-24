#include "Precompiled.h"
#include "GraphicsPrecompiled.h"
#include "ShaderCallbacks.h"

#include "System/Graphics.h"
#include "Core/CoreInterface.h"
#include "Math/MathInterface.h"
#include "Camera/Camera.h"
#include "Components/LightComponent.h"

#include "Graphics/Particles/ParticleEmitter.h"

#include "Graphics/Particles/ParticleDescription.h"


namespace WickedSick
{
  void LineShaderCallback(Graphics* gSys, void* data, std::vector<ParamPasser>& params)
  {
    Matrix4 modelToWorld;
    Camera* camera = gSys->GetCamera();
    modelToWorld.Identity();
    ModelComponent* comp = (ModelComponent*)data;
    Transform* tr = (Transform*) comp->GetSibling(CT_Transform);
    Matrix4 rotate = modelToWorld.GetRotatedXYZ(tr->GetRotation());
    Matrix4 scale = modelToWorld.GetScaled(tr->GetScale());
    Matrix4 translate = modelToWorld.GetTranslated(tr->GetPosition());

    modelToWorld = translate *  scale;

    Matrix4 worldToClip = gSys->GetProjection() * camera->GetViewMatrix();
    Vector3 cameraPos = camera->GetPosition();
    Vector4 color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);
    params.push_back(std::move(ParamPasser("DrawBuffer", "modelToWorld", &modelToWorld, sizeof(modelToWorld))));
    params.push_back(std::move(ParamPasser("DrawBuffer", "worldToClip", &worldToClip, sizeof(worldToClip))));
    params.push_back(std::move(ParamPasser("DrawBuffer", "color", &color, sizeof(color))));
    params.push_back(std::move(ParamPasser("DrawBuffer", "camPos", &cameraPos, sizeof(cameraPos))));
  }

  void vertexShaderCallback(Graphics* gSys, void* data, std::vector<ParamPasser>& params)
  {
    LightingShaderParam* callbackParams = (LightingShaderParam*)data;
    ModelComponent* comp = callbackParams->modelComp;
    Shader* shader = callbackParams->shader;
    shader->AddTexture(gSys->GetTexture(comp->GetTexture  ()));
    shader->AddTexture(gSys->GetTexture(comp->GetNormalMap()));

    if(comp)
    {
      Matrix4 modelToWorld;
      modelToWorld.Identity();
      Camera* camera = gSys->GetCamera();
      Transform* tr = (Transform*) comp->GetSibling(CT_Transform);

      
      Matrix4 rotate = modelToWorld.GetRotatedXYZ(tr->GetRotation());
      Matrix4 scale = modelToWorld.GetScaled(tr->GetScale());
      Matrix4 translate = modelToWorld.GetTranslated(tr->GetPosition());

      modelToWorld = translate * rotate * scale;

      Matrix4 worldToClip = gSys->GetProjection() * camera->GetViewMatrix();
      Vector3 cameraPos = camera->GetPosition();


      params.push_back(std::move(ParamPasser("TransformBuffer", "modelToWorld", &modelToWorld, sizeof(modelToWorld))));
      params.push_back(std::move(ParamPasser("TransformBuffer", "worldToClip", &worldToClip, sizeof(worldToClip))));
      const Material& mat = comp->GetMaterial();
      
      params.push_back(std::move(ParamPasser("ObjectProperties", "emissiveColor", &mat.emissiveColor, sizeof(mat.emissiveColor))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "diffuseColor", &mat.diffuseColor, sizeof(mat.diffuseColor))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "specularColor", &mat.specularColor, sizeof(mat.specularColor))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "ambientColor", &mat.ambientColor, sizeof(mat.ambientColor))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "emissiveConstant", &mat.emissiveConstant, sizeof(mat.emissiveConstant))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "diffuseConstant", &mat.diffuseConstant, sizeof(mat.diffuseConstant))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "specularConstant", &mat.specularConstant, sizeof(mat.specularConstant))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "ambientConstant", &mat.ambientConstant, sizeof(mat.ambientConstant))));
      params.push_back(std::move(ParamPasser("ObjectProperties", "shininess", &mat.shininess, sizeof(mat.shininess))));

      //if(camera->IsDirty())
      {
        params.push_back(std::move(ParamPasser("CameraBuffer", "cameraPos", &cameraPos, sizeof(cameraPos))));
        camera->Clean();
      }

      //if(gSys->LightsDirty())
      {
        auto& lights = gSys->GetLights();
        
        std::vector<Point> point;
        std::vector<Dir> dir;
        std::vector<Spot> spot;

        
        for(auto& it : lights)
        {
          switch(it->GetLightType())
          {
            case LightType::DirectionalLight:
            {
              dir.push_back(it->GetInfo().dir);
              break;
            }
            case LightType::PointLight:
            {
              point.push_back(it->GetInfo().point);
              break;
            }
            case LightType::SpotLight:
            {
              spot.push_back(it->GetInfo().spot);
              break;
            }
          }
        }
        int numDirLights = dir.size();
        int numSpotLights = spot.size();
        int numPointLights = point.size();

        //hacked as shit
        while(dir.size() < 8)
        {
          dir.push_back(Dir());
        }
        while(spot.size() < 8)
        {
          spot.push_back(Spot());
        }
        while(point.size() < 8)
        {
          point.push_back(Point());
        }

        
        
        params.push_back(std::move(ParamPasser("DirLightBuffer", "numDirLights", &numDirLights, sizeof(numDirLights))));
        if(numDirLights)
          params.push_back(std::move(ParamPasser("DirLightBuffer", "dirLights", &dir[0], sizeof(Dir) * dir.size())));
        params.push_back(std::move(ParamPasser("SpotLightBuffer", "numSpotLights", &numSpotLights, sizeof(numSpotLights))));
        if(numSpotLights)
          params.push_back(std::move(ParamPasser("SpotLightBuffer", "spotLights", &spot[0], sizeof(Spot) * spot.size())));
        params.push_back(std::move(ParamPasser("PointLightBuffer", "numPointLights", &numPointLights, sizeof(numPointLights))));
        if(numPointLights)
          params.push_back(std::move(ParamPasser("PointLightBuffer", "pointLights", &point[0], sizeof(Point) * point.size())));
        gSys->CleanLights();
      }
      
      //if(gSys->SceneDirty())
      {
        const SceneConstants& constants = gSys->GetSceneConstants();
        params.push_back(std::move(ParamPasser("ShaderGlobals", "globalAmbientColor", &constants.ambientColor, sizeof(constants.ambientColor))));
        params.push_back(std::move(ParamPasser("ShaderGlobals", "globalAmbientIntensity", &constants.ambientIntensity, sizeof(constants.ambientIntensity))));
        params.push_back(std::move(ParamPasser("ShaderGlobals", "fogColor", &constants.fogColor, sizeof(constants.fogColor))));
        params.push_back(std::move(ParamPasser("ShaderGlobals", "fogFar", &constants.fogFar, sizeof(constants.fogFar))));
        params.push_back(std::move(ParamPasser("ShaderGlobals", "fogNear", &constants.fogNear, sizeof(constants.fogNear))));
        gSys->CleanScene();
      }
      
    }
  }


  void ParticleShaderCallback(Graphics* gSys, void* data, std::vector<ParamPasser>& params)
  {
    ParticleShaderParam* particleParam = (ParticleShaderParam*)data;
    ParticleDescription* particle = particleParam->particle;
    EmitterDescription& emitterDesc = particleParam->emitter->GetEmitterDescription();
    ParticleSystem* system = particleParam->emitter->GetSystem();
    //Transform* tr = (Transform*)system->GetComponent()->GetSibling(CT_Transform);
    particleParam->shader->AddTexture(gSys->GetTexture(emitterDesc.sourceTexture));
    

    if(particle)
    {
      Vector4 particlePos(particle->Position);
      //particlePos += emitterDesc.spawnPos;
      //particlePos += system->GetSystemDescription().position;
      //particlePos +=
      Matrix4 modelToWorld;
      modelToWorld.Identity();
      Camera* camera = gSys->GetCamera();

      
      Matrix4 scale = modelToWorld.GetScaled(particle->Scale.xyz());
      Matrix4 translate = modelToWorld.GetTranslated(particlePos.xyz());
      modelToWorld = translate * scale;

      Matrix4 worldToCamera = camera->GetViewMatrix();
      Matrix4 cameraToClip = gSys->GetProjection();

      params.reserve(9);

      std::string bufferName;
      std::string paramName;
      bufferName = "MatrixBuf";
      paramName = "modelToWorld";
      params.push_back(ParamPasser(bufferName, paramName, &modelToWorld, sizeof(modelToWorld)));
      paramName = "worldToCamera";
      params.push_back(ParamPasser(bufferName, paramName, &worldToCamera, sizeof(worldToCamera)));
      paramName = "cameraToClip";
      params.push_back(ParamPasser(bufferName, paramName, &cameraToClip, sizeof(cameraToClip)));


      bufferName = "Particle";
      paramName = "Position";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Position, sizeof(particle->Position)));
      paramName = "Velocity";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Velocity, sizeof(particle->Velocity)));
      paramName = "Color";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Color, sizeof(particle->Color)));
      paramName = "Scale";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Scale, sizeof(particle->Scale)));
      paramName = "Rotation";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Rotation, sizeof(particle->Rotation)));
      paramName = "Lifetime";
      params.push_back(ParamPasser(bufferName, paramName, &particle->Lifetime, sizeof(particle->Lifetime)));
      paramName = "PrevPos";
      params.push_back(ParamPasser(bufferName, paramName, &particle->PrevPos, sizeof(particle->PrevPos)));

    }
  }

  void WireframeShaderCallback(Graphics* gSys, ModelComponent* comp, std::vector<ParamPasser>& params)
  {
    if(comp)
    {
      Matrix4 modelToWorld;
      Camera* camera = gSys->GetCamera();
      Transform* tr = (Transform*) comp->GetSibling(CT_Transform);

      modelToWorld.Identity();



      Matrix4 rotate = modelToWorld.GetRotatedXYZ(tr->GetRotation());
      Matrix4 scale = modelToWorld.GetScaled(tr->GetScale());
      Matrix4 translate = modelToWorld.GetTranslated(tr->GetPosition());

      modelToWorld = translate * rotate * scale;

      Matrix4 worldToClip = gSys->GetProjection() * camera->GetViewMatrix();
      Matrix4 modelToClip = worldToClip * modelToWorld;
      Vector4 color = Vector4(0.0f, 1.0f, 0.0f, 1.0f);


      params.push_back(ParamPasser("DrawBuffer", "modelToClip", &modelToClip, sizeof(modelToClip)));
      params.push_back(ParamPasser("DrawBuffer", "color", &color, sizeof(color)));
    }
  }

}


