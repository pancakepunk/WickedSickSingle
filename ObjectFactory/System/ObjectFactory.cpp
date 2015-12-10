#include "Precompiled.h"
#include "ObjectFactoryPrecompiled.h"
#include "ObjectFactory.h"

#include "EventSystem/EventSystemInterface.h"
#include "Core/System/System.h"

#include "Components/ComponentFactory.h"

#include "Components/ComponentManager.h"

#include "Graphics/GraphicsInterface.h"
#include "Physics/PhysicsInterface.h"



 

namespace WickedSick
{

  ObjectFactory::ObjectFactory() 
  : objects_created_(0),
    comp_manager_(nullptr),
    System(ST_ObjectFactory)
  {
  }

  ObjectFactory::~ObjectFactory()
  {

  }

  void ObjectFactory::Initialize()
  {
    comp_manager_ = new ComponentManager();
  }
  
  bool ObjectFactory::Load()
  {
    

    return true;
  }
  
  bool ObjectFactory::Reload()
  {
    return true;
  }
  
  void ObjectFactory::Update(float dt)
  {
    comp_manager_->Update(dt);
    for (auto& it : game_objects_)
    {
      Transform* tr = (Transform*)it.second->GetComponent(CT_Transform);
      PhysicsComponent* ph = (PhysicsComponent*)it.second->GetComponent(CT_PhysicsComponent);
      if (ph)
      {
        tr->SetPosition(ph->GetPosition());
      }
    }
  }

  void ObjectFactory::ReceiveMessage(Event * msg)
  {

  }

  GameObject* ObjectFactory::CloneArchetype(const std::string& name)
  {
    GameObject* newObject = object_factory_.Make(name);
    
    if(newObject)
    {
      const char* names[] = 
      {
      #define RegisterComponentType(x) #x,
      #include "Core/GameObject/ComponentTypes.h"
      #undef RegisterComponentType
      };
      newObject->AddComponent(comp_manager_->CreateComponent(names[0], newObject));
      for(int i = 1; i < CT_Count; ++i)
      {
        newObject->AddComponent(comp_manager_->CreateComponent(names[i], name, newObject));
      }
      
      newObject->SetID(objects_created_++);
      game_objects_.insert(std::make_pair(newObject->GetID(), newObject));
      newObject->SetArchetypeName(name);
      newObject->Initialize();
      newObject->Activate();
    }
    return newObject;
  }

  GameObject* ObjectFactory::MakeBlank()
  {
    return object_factory_.MakeBlank();
  }

  ComponentManager * ObjectFactory::GetComponentManager()
  {
    return comp_manager_;
  }

  std::unordered_map<std::string, Archetype<GameObject> >& ObjectFactory::GetArchetypes()
  {
    return object_factory_.GetArchetypes();
  }

  GameObject* ObjectFactory::GetObject(int id)
  {
    auto object = game_objects_.find(id);
    if(object != game_objects_.end())
    {
      return object->second;
    }
    return nullptr;
  }

  void ObjectFactory::CreateArchetypes()
  {
    ComponentFactory<ModelComponent>* modelFactory = (ComponentFactory<ModelComponent>*)comp_manager_->GetFactory("ModelComponent");
    ComponentFactory<DemoComponent>* demoFactory = (ComponentFactory<DemoComponent>*)comp_manager_->GetFactory("DemoComponent");
    ComponentFactory<PhysicsComponent>* physicsFactory = (ComponentFactory<PhysicsComponent>*)comp_manager_->GetFactory("PhysicsComponent");
    ComponentFactory<OrbitComponent>* orbitFactory = (ComponentFactory<OrbitComponent>*)comp_manager_->GetFactory("OrbitComponent");
    ComponentFactory<LightComponent>* lightFactory = (ComponentFactory<LightComponent>*)comp_manager_->GetFactory("LightComponent");
    ComponentFactory<ParticleComponent>* particleFactory = (ComponentFactory<ParticleComponent>*)comp_manager_->GetFactory("ParticleComponent");
    ComponentFactory<CameraController>* controllerFactory = (ComponentFactory<CameraController>*)comp_manager_->GetFactory("CameraController");
    ComponentFactory<CameraComponent>* cameraFactory = (ComponentFactory<CameraComponent>*)comp_manager_->GetFactory("CameraComponent");
    ComponentFactory<SkyboxComponent>* skyboxFactory = (ComponentFactory<SkyboxComponent>*)comp_manager_->GetFactory("SkyboxComponent");
    ComponentFactory<ReflectComponent>* relfectFactory = (ComponentFactory<ReflectComponent>*)comp_manager_->GetFactory("ReflectComponent");

    

    modelFactory->AddArchetype("reflectbox", Archetype<ModelComponent>("reflectbox"));
    modelFactory->AddArchetype("bunny", Archetype<ModelComponent>("bunny"));
    modelFactory->AddArchetype("box", Archetype<ModelComponent>("box"));
    modelFactory->AddArchetype("skybox", Archetype<ModelComponent>("skybox"));
    modelFactory->AddArchetype("sphere", Archetype<ModelComponent>("sphere"));
    modelFactory->AddArchetype("plane", Archetype<ModelComponent>("plane"));
    modelFactory->AddArchetype("dirLight", Archetype<ModelComponent>("dirLight"));
    
    demoFactory->AddArchetype("box", Archetype<DemoComponent>("box"));
    demoFactory->AddArchetype("reflectbox", Archetype<DemoComponent>("reflectbox"));
    demoFactory->AddArchetype("skybox", Archetype<DemoComponent>("skybox"));

    physicsFactory->AddArchetype("bunny", Archetype<PhysicsComponent>("bunny"));
    physicsFactory->AddArchetype("box", Archetype<PhysicsComponent>("box"));
    physicsFactory->AddArchetype("skybox", Archetype<PhysicsComponent>("skybox"));
    physicsFactory->AddArchetype("sphere", Archetype<PhysicsComponent>("sphere"));


    //orbitFactory->AddArchetype("sphere", Archetype<OrbitComponent>("sphere"));
    lightFactory->AddArchetype("dirLight", Archetype<LightComponent>("dirLight"));

    cameraFactory->AddArchetype("camera", Archetype<CameraComponent>("camera"));

    skyboxFactory->AddArchetype("skybox", Archetype<SkyboxComponent>("skybox"));

    controllerFactory->AddArchetype("camera", Archetype<CameraController>("camera"));

    particleFactory->AddArchetype("fire", Archetype<ParticleComponent>("fire"));

    object_factory_.AddArchetype("dirLight", Archetype<GameObject>("dirLight"));
    object_factory_.AddArchetype("bunny", Archetype<GameObject>("bunny"));
    object_factory_.AddArchetype("plane", Archetype<GameObject>("plane"));
    object_factory_.AddArchetype("box", Archetype<GameObject>("box"));
    object_factory_.AddArchetype("reflectbox", Archetype<GameObject>("reflectbox"));
    object_factory_.AddArchetype("skybox", Archetype<GameObject>("skybox"));
    object_factory_.AddArchetype("camera", Archetype<GameObject>("camera"));
    object_factory_.AddArchetype("sphere", Archetype<GameObject>("sphere"));
    object_factory_.AddArchetype("fire", Archetype<GameObject>("fire"));

    relfectFactory->AddArchetype("reflectbox", Archetype<ReflectComponent>("reflectbox"));


    
    WickedSick::Graphics* gSys = (WickedSick::Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);



    Archetype<ModelComponent>& bunnyCompType = modelFactory->GetArchetype("bunny");
    ModelComponent& bunnyModelComp = bunnyCompType.GetBase();
    bunnyModelComp.SetModel("bunny");

    Archetype<ModelComponent>& planeCompType = modelFactory->GetArchetype("plane");
    ModelComponent& planeModelComp = planeCompType.GetBase();
    planeModelComp.SetModel("plane");

    Archetype<ModelComponent>& dirLightCompType = modelFactory->GetArchetype("dirLight");
    ModelComponent& dirLightModelComp = dirLightCompType.GetBase();
    dirLightModelComp.SetModel("sphere");

    Archetype<ModelComponent>& cubeCompType = modelFactory->GetArchetype("box");
    ModelComponent& cubeModelComp = cubeCompType.GetBase();
    cubeModelComp.SetModel("box");

    Archetype<ModelComponent>& skyboxCompType = modelFactory->GetArchetype("skybox");
    ModelComponent& skyboxModelComp = skyboxCompType.GetBase();
    skyboxModelComp.SetModel("skybox");

    Archetype<ModelComponent>& reflectboxCompType = modelFactory->GetArchetype("reflectbox");
    ModelComponent& reflectboxModelComp = reflectboxCompType.GetBase();
    reflectboxModelComp.SetModel("box");
    
    Archetype<ModelComponent>& sphereCompType = modelFactory->GetArchetype("sphere");
    ModelComponent& sphereModelComp = sphereCompType.GetBase();
    sphereModelComp.SetModel("sphere");
    

    Archetype<CameraComponent>& cameraCompType = cameraFactory->GetArchetype("camera");
    CameraComponent& cameraTemplateComp = cameraCompType.GetBase();
    cameraTemplateComp.SetLookAt(Vector3(0.0f, 0.0f, 0.0f));


    Archetype<PhysicsComponent>& spherePhysCompType = physicsFactory->GetArchetype("sphere");
    PhysicsComponent& spherePhysComp = spherePhysCompType.GetBase();
    spherePhysComp.GetRigidBody()->SetGravityScalar(0.0f);


    

    Archetype<ParticleComponent>& fireCompType = particleFactory->GetArchetype("fire");
    ParticleComponent& fireParticleComp = fireCompType.GetBase();


    auto particleManager = gSys->GetParticleManager();
    SystemDescription desc;
    desc.lifetime = 0.0f;
    desc.name = "fire";
    desc.position = Vector3();

    ParticleSystem* sysToClone = particleManager->MakeParticleSystem(desc);

    fireParticleComp.AddParticleSystem(sysToClone);
    EmitterDescription emitterDesc;
    emitterDesc.maxParticles = 1000;
    emitterDesc.emitLength = 0.05f;
    emitterDesc.frequency = 0.1f;
    emitterDesc.lazy = false;
    emitterDesc.amountToSpawn = 2;
    emitterDesc.lifetime = 0.0f;
    emitterDesc.sourceModel = "quad";
    emitterDesc.sourceTexture = "feather";
    emitterDesc.spawnPos = Vector3(0.0f, 1.0f, 0.0f);
    ParticleEmitter* emitter = particleManager->MakeParticleEmitter(emitterDesc);

    ParticleDescription states[3];
    states[0] = ParticleDescription(Vector3(0.0f, 5.0f, 0.0f),
                                    Vector3(0.0f, 1.0f, 0.0f),
                                    Vector4(1.0f, 1.0f, 1.0f, 0.5f),
                                    Vector3(0.5f, 0.5f, 0.5f),
                                    0.0f,
                                    2.0f);
    states[1] = ParticleDescription(Vector3(0.0f, 5.0f, 0.0f),
                                    Vector3(0.0f, 0.5f, 0.0f),
                                    Vector4(1.0f, 0.25f, 0.25f, 0.5f),
                                    Vector3(0.25f, 0.25f, 0.25f),
                                    0.0f,
                                    2.0f);

    states[2] = ParticleDescription(Vector3(0.0f, 5.0f,  0.0f),
                                    Vector3(0.0f, 0.25f, 0.0f),
                                    Vector4(1.0f, 1.0f, 0.1f, 0.5f),
                                    Vector3(0.05f, 0.05f, 0.05f),
                                    0.0f,
                                    2.0f);


    ParticleDescription variance[3];
    variance[0] = ParticleDescription(Vector3(0.1f, 0.25f, 0.1f),
                                      Vector3(0.25f, 0.25f, 0.25f),
                                      Vector4(),
                                      Vector3(),
                                      0.0f,
                                      0.1f);
    variance[1] = ParticleDescription(Vector3(0.1f, 0.1f, 0.1f),
                                      Vector3(0.25f, 0.25f, 0.25f),
                                      Vector4(),
                                      Vector3(),
                                      0.0f,
                                      0.1f);

    variance[2] = ParticleDescription(Vector3(0.1f, 0.1f, 0.1f),
                                      Vector3(0.1f, 0.1f, 0.1f),
                                      Vector4(),
                                      Vector3(),
                                      0.0f,
                                      0.1f);

    emitter->AddParticleState(states[0], variance[0]);
    emitter->AddParticleState(states[1], variance[1]);
    emitter->AddParticleState(states[2], variance[2]);
    //emitter->RegisterAttribute("Velocity");
    //emitter->RegisterAttribute("Lifetime");
    emitter->RegisterAttribute("Color");
    emitter->RegisterAttribute("Scale");


    sysToClone->AddEmitter(emitter);
    

    


    

  }

  void ObjectFactory::BuildScene()
  {

    GameObject* camera = CloneArchetype("camera");
    Transform* cameraTr = (Transform*)camera->GetComponent(CT_Transform);
    cameraTr->SetRotation(0.0f, PI / 4.0f, 0.0f);
    cameraTr->SetPosition(0.0f, 10.0f, -10.0f);
    CameraComponent* cameraComp = (CameraComponent*)camera->GetComponent(CT_CameraComponent);

    


    Graphics* graphics = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    graphics->GetCamera()->SetSource((CameraComponent*)cameraComp);






    GameObject* plane = CloneArchetype("plane");
    Transform* planeTr = (Transform*) plane->GetComponent(CT_Transform);
    //bunnyTr->SetRotation(0.0f, PI/4.0f, 0.0f);
    planeTr->SetPosition(0.0f, -18.0f, 0.0f);
    planeTr->SetScale(15.0f);
    ModelComponent* planeModel = (ModelComponent*) plane->GetComponent(CT_ModelComponent);
    planeModel->SetDrawType(DrawType::Default);
    planeModel->SetShader("pixelblinn");
    planeModel->SetTexture("white");
    Material& planemat = planeModel->GetMaterial();

    planemat.ambientColor = Vector4(0.2f, 0.2f, 0.2f, 1.0f);
    planemat.specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    planemat.emissiveColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    planemat.diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    planemat.ambientConstant = 0.3f;
    planemat.specularConstant = 1.0f;
    planemat.emissiveConstant = 0.3f;
    planemat.diffuseConstant = 0.5f;
    planemat.shininess = 20.0f;



    GameObject* fire = CloneArchetype("fire");
    Transform* fireTr = (Transform*) fire->GetComponent(CT_Transform);
    
    fireTr->SetPosition(0.0f, 0.0f, 0.0f);
    
    ParticleComponent* fireParticle = (ParticleComponent*) fire->GetComponent(CT_ParticleComponent);


    





    GameObject* skybox = CloneArchetype("skybox");
    Transform* skyboxTr = (Transform*) skybox->GetComponent(CT_Transform);
    //bunnyTr->SetRotation(0.0f, PI/4.0f, 0.0f);
    skyboxTr->SetPosition(0.0f, 0.0f, 0.0f);
    skyboxTr->SetScale(50000.0f);
    ModelComponent* skyboxModel = (ModelComponent*) skybox->GetComponent(CT_ModelComponent);
    skyboxModel->SetDrawType(DrawType::Default);
    skyboxModel->SetShader("skybox");
    //skyboxModel->SetTexture("bettercheckerboard");
    //skyboxModel->SetNormalMap("white");
    Material& skyboxmat = skyboxModel->GetMaterial();

    skyboxmat.ambientColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    skyboxmat.specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    skyboxmat.emissiveColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    skyboxmat.diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    skyboxmat.ambientConstant = 0.3f;
    skyboxmat.specularConstant = 1.0f;
    skyboxmat.emissiveConstant = 0.3f;
    skyboxmat.diffuseConstant = 1.0f;
    skyboxmat.shininess = 20.0f;

    PhysicsComponent* skyboxPhysics = (PhysicsComponent*) skybox->GetComponent(CT_PhysicsComponent);
    RigidBody* skyboxBody = skyboxPhysics->GetRigidBody();
    skyboxBody->SetGravityScalar(0.0f);


    
    
    Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);
    for(int i = 0; i < 1; ++i)
    {
      Vector3 fromBunny = Vector3(8.0f, 0.0f, 8.0f);
      GameObject* dirLight = CloneArchetype("dirLight");
      LightComponent* dirLightlight = (LightComponent*) dirLight->GetComponent(CT_LightComponent);
      Transform* tr = (Transform*)dirLight->GetComponent(CT_Transform);
      LightInfo& info = dirLightlight->GetInfo();
      dirLightlight->SetLightType(LightType::PointLight);
      info.dir.ambientIntensity.Zero();
      info.dir.diffuseIntensity.Zero();
      info.dir.specularIntensity.Zero();
      info.spot.ambientIntensity.Zero();
      info.spot.diffuseIntensity.Zero();
      info.spot.specularIntensity.Zero();
    
      if(i > 0)
      {
        info.dir.ambientIntensity.Zero();
        info.dir.diffuseIntensity.Zero();
        info.dir.specularIntensity.Zero();
      }
      else
      {
        info.point.ambientIntensity = {0.2f, 0.2f, 0.2f};
        info.point.diffuseIntensity = {1.0f, 1.0f, 1.0f};
        info.point.specularIntensity = {1.0f, 1.0f, 1.0f};
        info.point.attenuationConstants = {1.0f, 0.1f, 0.0f};
      }
    
      info.dir.direction = Vector3(-1.0f, -1.0f, -1.0f);
      info.spot.direction = Vector3(-1.0f, -1.0f, -1.0f);
      //info.spot.theta = 35.0f * PI / 180.0f;
      //info.spot.phi = 15.0f * PI / 180.0f;
    
      tr->SetPosition(RotateAround(axis, (i * 45.0f) * PI / 180.0f, fromBunny));
      ModelComponent* lightModel = (ModelComponent*) dirLight->GetComponent(CT_ModelComponent);
      lightModel->SetDrawType(DrawType::Default);
      lightModel->SetShader("flat");
      lightModel->SetTexture("white");
      tr->SetScale(0.5f);
    }


    
    
   

    //GameObject* box = CloneArchetype("sphere");
    //Transform* boxTr = (Transform*)box->GetComponent(CT_Transform);
    //boxTr->SetRotation(0.0f, PI / 4.0f, 0.0f);
    //boxTr->SetScale(1.0f);
    //boxTr->SetPosition(0.0f, 0.0f, 6.0f);
    //PhysicsComponent* orbit = (PhysicsComponent*)box->GetComponent(CT_PhysicsComponent);
    //orbit->GetRigidBody()->SetGravityScalar(0.0f);

    //bunnyTr->SetScale(100.0f);
  }

}

