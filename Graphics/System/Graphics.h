#pragma once



#include "API/GraphicsAPI.h"
#include "General/GraphicsOptions.h"
#include "Model/Model.h"
#include "SceneConstants.h"


#include "Core/CoreInterface.h"
#include "Math/MathInterface.h"
#include "Utility/DebugGeometry.h"
#include "Components/ModelComponent.h"


#include "Utility/UtilityInterface.h"
struct ImDrawData;
namespace WickedSick
{
  class ModelComponent;
  class LightComponent;
  class MatrixStack;
  class Shader;
  class Model;
  class ModelLoader;
  class Camera;
  struct Device;
  struct SwapChain;
  class ParticleManager;

  

  class Graphics : public System
  {
  public:
    Graphics();
    ~Graphics();
    void Initialize();
    bool Load();
    bool Reload();
    void Update(float dt);
    void ReceiveMessage(Event * msg);
    void Render();

    Camera* GetCamera();

    Model* GetModel(const std::string& name);
    Model* LoadModel(const std::string& model);
    Texture* GetTexture(const std::string& tex);
    Texture* LoadTexture(const std::string& tex);
    Texture* LoadBumpMap(const std::string& map);

    ParticleManager* GetParticleManager();

    void AddShader(const std::string& name, 
                   Shader::ShaderCallback callback);

    void RecompileShaders();

    void BeginScene();
    void EndScene();

    void RenderDebug();
    void PrepareDebug(ModelComponent * debugModel);


    void AddDebugModel(ModelComponent * debug);


    Matrix4 GetProjection();

    Shader* GetShader(const std::string& name);
    Renderable* GetRenderable(const std::string& name);

    void AddLight(LightComponent* newLight);
    void RemoveLight(LightComponent* toKill);
    void DirtyLights();
    bool LightsDirty();
    void CleanLights();
    std::set<LightComponent*>& GetLights();


    void DirtyScene();
    void CleanScene();
    bool SceneDirty();
    SceneConstants& GetSceneConstants();

    RenderTarget* GetRenderTarget(size_t index);
    std::vector<RenderTarget*>& GetRenderTargets();

    void DoPass();


    void SetBackBuffer(RenderTarget* backBuf);
    
    static GraphicsAPI*            graphicsAPI;
  private:


    bool repass_lights_;
    bool reset_scene_constants_;
    SceneConstants scene_constants_;

   
    std::set<LightComponent*> light_components_;

    std::vector<ModelComponent*> debug_models_;
    HashMap<int, ModelComponent*> model_components_; 
    HashMap<std::string, ModelLoader*> loaders_;
    HashMap<std::string, Model*> models_;
    HashMap<std::string, Renderable> renderables_;//basically buffers that don't have associated models
    HashMap<std::string, Shader*> shaders_;
    HashMap<std::string, Texture*> textures_;

    RenderTarget* back_buffer_;
    std::vector<RenderTarget*> render_targets_;


    std::vector<Vertex> debug_lines_;
    Buffer* debug_buffer_;    

    Matrix4 projection_matrix_;
    Matrix4 orthographic_matrix_;

    
    ParticleManager*  particle_manager_;

    MatrixStack*      mat_stack_;

    Camera*           camera_;

    GraphicsOptions*  options_;

    bool draw_ui_;

    float recompile_timer_;


  };
}
