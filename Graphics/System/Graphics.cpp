#include "GraphicsPrecompiled.h"
#include "Graphics.h"

#include "../D3D/Device.h"
#include "../D3D/SwapChain.h"

#include "Core/CoreInterface.h"
#include "Window/WindowInterface.h"

#include "API/DirectX.h"

#include "Camera/Camera.h"

#include "Model/Model.h"
#include "Shader/Shader.h"
#include "Model/Loaders/ObjLoader.h"
#include "Model/Loaders/BinLoader.h"


#include "Components/ModelComponent.h"

#include "Utility/UtilityInterface.h"

#include "Shader/ShaderCallbacks.h"

#include "General/GraphicsTypes.h"

#include "Graphics/Particles/ParticleManager.h"


#include "Input/InputInterface.h"

#include "anttweakbar/include/AntTweakBar.h"



#include <thread>
#include <mutex>

namespace WickedSick
{
  GraphicsAPI*  Graphics::graphicsAPI = nullptr;
  Graphics::Graphics() 
  : camera_(nullptr),
    options_(nullptr),
    mat_stack_(nullptr),
    debug_buffer_(nullptr),
    repass_lights_(true),
    reset_scene_constants_(true),
    particle_manager_(nullptr),
    draw_ui_(false),
    System(ST_Graphics)
  {
    loaders_.insert(".obj", new ObjLoader());
    loaders_.insert(".bin", new BinLoader());

    mat_stack_ = new MatrixStack();

    camera_ = new Camera();
    options_ = new GraphicsOptions();

    particle_manager_ = new ParticleManager();

    switch (options_->Api)
    {
      case APIType::OpenGLAPI:
        //shit aint happening right now
        WSError("Yeah no OpenGL support yet sorry");
        break;
      case APIType::DirectX:
        graphicsAPI = new DirectX(); 
        break;
      default:
        WSError("What are you even what dude what. Whatever that is, we don't support it.");
        break;
    }
    scene_constants_.ambientIntensity = 0.5f;
    scene_constants_.ambientColor = Vector4(0.1f, 0.1f, 0.1f, 1.0f);
  }

  Graphics::~Graphics()
  {
    delete mat_stack_;
    delete camera_;
    delete options_;
    delete graphicsAPI;
    for(auto& it : loaders_)
    {
      delete (it.val);
    }
    TwTerminate();
  }

  void Graphics::Initialize()
  {

    
    
    WickedSick::Window* window = core_->GetSystem<WickedSick::Window>(ST_Window);
    std::lock_guard<std::mutex> lk(window->GetWindowHandleMutex());

    graphicsAPI->Initialize(options_, window);

    // Setup the projection matrix.
    float fieldOfView = (float)PI / 2.0f;//90 degrees
    float screenAspect = (float)window->GetWindowSize().x / (float)window->GetWindowSize().y;

    // Create the projection matrix for 3D rendering.
    projection_matrix_.DoPerspective(90.0 * PI/180.0, 0.1f, 100000.0f, screenAspect);
    orthographic_matrix_.DoOrthographic(window->GetWindowSize().x, window->GetWindowSize().y, 0.1f, 100000.0f);
    camera_->SetPosition(0.0f, 0.0f, 0.0f);

    mat_stack_->Push(projection_matrix_);
    
    

    
    
  }

  bool Graphics::Load()
  {
    //temporary init
    LoadModel("box.bin");
    LoadModel("bunny.bin");
    LoadModel("sphere.bin");
    LoadModel("plane.bin");
    LoadModel("dragon.bin");
    LoadModel("horse.bin");

    LoadTexture("white.png");
    LoadTexture("cat_test.png");

    AddShader("flat", &vertexShaderCallback);
    AddShader("vertexblinn", &vertexShaderCallback);
    AddShader("lines", &LineShaderCallback, false);
    AddShader("vertexphong", &vertexShaderCallback);
    AddShader("pixelphong", &vertexShaderCallback);
    AddShader("pixelblinn", &vertexShaderCallback);
    AddShader("particle", &ParticleShaderCallback);
    //AddShader("wireframe", &WireframeShaderCallback);
    

    EmitterDescription emitterDesc;
    emitterDesc.emitLength = 0.05f;
    emitterDesc.frequency = 0.1f;
    emitterDesc.lazy = false;
    emitterDesc.amountToSpawn = 12;
    emitterDesc.lifetime = 0.0f;
    emitterDesc.sourceModel = "sphere";
    emitterDesc.sourceTexture = "white";
    emitterDesc.spawnPos = Vector3(10.0f, 2.0f, 10.0f);
    ParticleEmitter* emitter = particle_manager_->MakeParticleEmitter(1000, emitterDesc);

    ParticleDescription states[3];
    states[0] = ParticleDescription(Vector4(0.0f, 5.0f, 0.0f, 1.0f),
                                   Vector4(0.0f, 1.0f, 0.0f, 1.0f),
                                   Vector4(1.0f, 1.0f, 1.0f, 1.0f),
                                   Vector4(0.5f, 0.5f, 0.5f, 0.5f),
                                   0.0f, 
                                   5.0f);
    states[1] = ParticleDescription(Vector4(0.0f, 5.0f, 0.0f, 1.0f),
                                   Vector4(0.0f, 0.5f, 0.0f, 1.0f),
                                   Vector4(1.0f, 0.25f, 0.25f, 0.0f),
                                   Vector4(0.25f, 0.25f, 0.25f, 0.0f),
                                   0.0f,
                                   5.0f);

    states[2] = ParticleDescription(Vector4(0.0f, 5.0f, 0.0f, 1.0f),
                                   Vector4(0.0f, 0.25f, 0.0f, 1.0f),
                                   Vector4(1.0f, 1.0f, 0.1f, 0.0f),
                                   Vector4(0.05f, 0.05f, 0.05f, 0.0f),
                                   0.0f,
                                   5.0f);


    ParticleDescription variance[3];
    variance[0] = ParticleDescription(Vector4(0.1f, 0.25f, 0.1f, 0.0f),
                                     Vector4(0.25f, 0.25f, 0.25f, 1.0f),
                                     Vector4(),
                                     Vector4(),
                                     0.0f,
                                     0.1f);
    variance[1] = ParticleDescription(Vector4(0.1f, 0.1f, 0.1f, 1.0f),
                                     Vector4(0.25f, 0.25f, 0.25f, 1.0f),
                                     Vector4(),
                                     Vector4(),
                                     0.0f,
                                     0.1f);

    variance[2] = ParticleDescription(Vector4(0.1f, 0.1f, 0.1f, 1.0f),
                                     Vector4(0.1f, 0.1f, 0.1f, 1.0f),
                                     Vector4(),
                                     Vector4(),
                                     0.0f,
                                     0.1f);

    emitter->AddParticleState(states[0], variance[0]);
    emitter->AddParticleState(states[1], variance[1]);
    emitter->AddParticleState(states[2], variance[2]);
    emitter->RegisterAttribute("Velocity");
    emitter->RegisterAttribute("Lifetime");
    emitter->RegisterAttribute("Color");
    emitter->RegisterAttribute("Scale");
    


    return true;
  }

  bool Graphics::Reload()
  {
    return true;
  }

  void Graphics::Update(float dt)
  {
    RecompileShaders();
    particle_manager_->Update(dt);
    camera_->Orient(dt);
    Render();

    Input* input = (Input*) Engine::GetCore()->GetSystem(ST_Input);
    InputHandler* handler = input->GetHandler();
    if(handler->Check("UIToggle"))
    {
      draw_ui_ = !draw_ui_;
    }

  }

  void Graphics::ReceiveMessage(Event * msg)
  {

  }


  Model* Graphics::LoadModel(const std::string& modelName)
  {
    Model* newModel = nullptr;
    int dotIndex = modelName.find_last_of('.');
    std::string extension = modelName.substr(dotIndex, (modelName.size()) - dotIndex);
    auto& modelLoader = loaders_.find(extension);

    std::string friendlyName = modelName.substr(0, dotIndex);
    

    if (modelLoader != loaders_.end())
    {
      newModel = ((*modelLoader).val)->Load("Content/Models/" + modelName);
      if (newModel)
      {
        auto it = models_.find(friendlyName);
        if(it == models_.end())
        {
          models_.insert(friendlyName, newModel);
        }
        else
        {
          auto sit = models_.find(friendlyName + extension);
          if(sit == models_.end())
          {
            models_.insert(friendlyName + extension, newModel);
          }
          else
          {
            ConsolePrint("Two model files of same name and same type, cannot load model" + friendlyName + extension + "\n");
          }
        }
      }
    }
    else
    {
      ConsolePrint("Can't load file extension " + extension + " as model.\n");
    }
    newModel->Initialize();
    return newModel;
  }

  Texture* Graphics::GetTexture(const std::string & tex)
  {
    auto& texIt = textures_.find(tex);
    return (texIt != textures_.end()) ? (*texIt).val : nullptr;
  }

  Texture* Graphics::LoadTexture(const std::string& tex)
  {
    Texture* newTex = graphicsAPI->MakeTexture("Content/Textures/" + tex);
    textures_.insert(newTex->GetName(), newTex);
    newTex->Initialize();
    return newTex;
  }

  void Graphics::AddShader(const std::string& name, Shader::ShaderCallback callback, bool indexed)
  { 
    Shader* shader = graphicsAPI->MakeShader(name, callback, indexed);
    shader->Initialize();
    shaders_.insert(shader->GetName(), shader);
  }

  void Graphics::RecompileShaders()
  {
    for (auto & it : shaders_)
    {
      it.val->Compile();
    }
  }

  void Graphics::BeginScene()
  {
    
    std::string shader;
    for(auto it : models_)
    {
      auto& modelInstances = it.val->GetInstances();
      for(auto& inst : modelInstances)
      {
        shader = inst->GetShader();
        auto shaderIt = shaders_.find(shader);
        auto lineShader = shaders_.find("lines");
        auto wireFrameShader = shaders_.find("wireframe");
        switch(inst->GetDrawType())
        {
          case DrawType::Default:
          {
            if(shaderIt != shaders_.end())
            {
              (*shaderIt).val->AddInstance(inst);
            }
            break;
          }
          case DrawType::VertexNormals:
          {
            if(shaderIt != shaders_.end())
            {
              (*shaderIt).val->AddInstance(inst);
            }
            if(lineShader != shaders_.end())
            {
              (*lineShader).val->AddInstance(inst);
            }

            break;
          }
          case DrawType::FaceNormals:
          {
            if(shaderIt != shaders_.end())
            {
              (*shaderIt).val->AddInstance(inst);
            }
            if(lineShader != shaders_.end())
            {
              (*lineShader).val->AddInstance(inst);
            }
            
            break;
          }
          case DrawType::Wireframe:
          {
            //(*wireFrameShader).val->AddInstance(inst);
            break;
          }

        }
      }
    }
    graphicsAPI->BeginScene();
    
  }

  void Graphics::EndScene()
  {
    graphicsAPI->EndScene();
    debug_models_.clear();
    debug_lines_.clear();
    for(auto it : shaders_)
    {
      it.val->ClearInstances();
    }
  }


  void Graphics::RenderDebug()
  {
    graphicsAPI->RenderDebug(debug_buffer_);
  }

  void Graphics::PrepareDebug(ModelComponent* debugModel)
  {
    debug_lines_ = debugModel->GetDebugVerts();
    graphicsAPI->PrepareDebug(debug_buffer_, 
                              (debug_lines_.size()) ? (Vertex*)&debug_lines_[0] : nullptr, 
                              debug_lines_.size() * sizeof(Vertex));
  }

  void Graphics::AddDebugModel(ModelComponent* debug)
  {
    debug_models_.push_back(debug);
  }


  Matrix4 Graphics::GetProjection()
  {
    return projection_matrix_;
  }

  Shader * Graphics::GetShader(const std::string & name)
  {
    auto& shaderIt = shaders_.find(name);
    if(shaderIt != shaders_.end())
    {
      return (*shaderIt).val;
    }
    return nullptr;
  }

  Renderable * Graphics::GetRenderable(const std::string & name)
  {
    auto& renderableIt = renderables_.find(name);
    if(renderableIt != renderables_.end())
    {
      return &(*renderableIt).val;
    }
    return nullptr;
  }

  void Graphics::AddLight(LightComponent * newLight)
  {
    light_components_.insert(newLight);
  }

  void Graphics::RemoveLight(LightComponent * toKill)
  {
    light_components_.erase(toKill);
  }

  void Graphics::DirtyLights()
  {
    repass_lights_ = true;
  }

  bool Graphics::LightsDirty()
  {
    return repass_lights_;
  }

  void Graphics::CleanLights()
  {
    repass_lights_ = false;
  }

  void Graphics::DirtyScene()
  {
    reset_scene_constants_ = true;
  }

  bool Graphics::SceneDirty()
  {
    return reset_scene_constants_;
  }

  SceneConstants & Graphics::GetSceneConstants()
  {
    return scene_constants_;
  }

  void Graphics::CleanScene()
  {
    reset_scene_constants_ = false;
  }

  std::set<LightComponent*>& Graphics::GetLights()
  {
    return light_components_;
  }
  
  void Graphics::Render()
  {
    mat_stack_->Push(camera_->GetViewMatrix());
    scene_constants_.fogNear = std::max(scene_constants_.fogNear, 0.5f);
    scene_constants_.fogFar = std::max(scene_constants_.fogFar, scene_constants_.fogNear + 1.0f);
    

    Matrix4 modelToWorld;
    BeginScene();
    
    LightingShaderParam paramList;
    int count = 0;
    Shader* debugShader = nullptr;
    for(auto& shaderIt : shaders_)
    {
      Shader* shader = shaderIt.val;
      if(shaderIt.key != "lines" && shaderIt.key != "particle")
      {
        Model* model = nullptr;
        for(auto& modelIt : shader->GetInstances())
        {
          model = modelIt.first;
          model->Render();

          for(auto& inst : modelIt.second)
          {
            
            paramList.modelComp = inst;
            paramList.shader = shader;

            shader->PrepareBuffers((void*)&paramList);

            shader->Render(model->GetNumIndices());
          }
        }
      }
      else
      {
        debugShader = shader;
      }
    }

    auto& particleSystems = particle_manager_->GetEmitters();
    auto shaderIt = shaders_.find("particle");
    Shader* particleShader = nullptr;
    if(shaderIt != shaders_.end())
    {
      particleShader = (*shaderIt).val;
    }
    
    ParticleShaderParam particleParam;
    particleParam.shader = particleShader;
    Model* particleModel;
    for(auto& it : particleSystems)
    {
      particleModel = GetModel(it->GetEmitterDescription().sourceModel);
      particleModel->Render();
      auto& particles = it->GetParticles();
      particleParam.emitter = &(it->GetEmitterDescription());
      for(auto& particleIndex : it->GetAlive())
      {
        particleParam.particle = &particles[particleIndex];
        particleShader->PrepareBuffers(&particleParam);
        particleShader->Render(particleModel->GetNumIndices());
      }
    }

    if(debugShader)
    {
      for(auto& it : debug_models_)
      {
        if(it->GetDrawType() != DrawType::Default)
        {
          PrepareDebug(it);
          RenderDebug();
          debugShader->PrepareBuffers(it);
          debugShader->Render(debug_lines_.size());
        }
      }
    }

    if(draw_ui_)  
    {
      TwDraw();
    }
    
    EndScene();
    mat_stack_->Pop();

  }

  Camera * Graphics::GetCamera()
  {
    return camera_;
  }

  Model* Graphics::GetModel(const std::string& name)
  {
    auto& modelIt = models_.find(name);
    return (modelIt != models_.end()) ? (*modelIt).val : nullptr;
  }

}
