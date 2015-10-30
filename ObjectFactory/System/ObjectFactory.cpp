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

  ObjectFactory::ObjectFactory() : objects_created_(0),
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
      newObject->Activate();
      newObject->SetArchetypeName(name);
      newObject->Initialize();
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
    ComponentFactory<PhysicsComponent>* physicsFactory = (ComponentFactory<PhysicsComponent>*)comp_manager_->GetFactory("PhysicsComponent");
    ComponentFactory<OrbitComponent>* orbitFactory = (ComponentFactory<OrbitComponent>*)comp_manager_->GetFactory("OrbitComponent");
    ComponentFactory<LightComponent>* lightFactory = (ComponentFactory<LightComponent>*)comp_manager_->GetFactory("LightComponent");
    
    physicsFactory->AddArchetype("bunny", Archetype<PhysicsComponent>("bunny"));
    physicsFactory->AddArchetype("box", Archetype<PhysicsComponent>("box"));
    physicsFactory->AddArchetype("sphere", Archetype<PhysicsComponent>("sphere"));
    orbitFactory->AddArchetype("sphere", Archetype<OrbitComponent>("sphere"));
    lightFactory->AddArchetype("dirLight", Archetype<LightComponent>("dirLight"));

    modelFactory->AddArchetype("bunny", Archetype<ModelComponent>("bunny"));
    Archetype<ModelComponent>& bunnyCompType = modelFactory->GetArchetype("bunny");
    ModelComponent& bunnyModelComp = bunnyCompType.GetBase();
    bunnyModelComp.SetModel("bunny");

    
    

    modelFactory->AddArchetype("plane", Archetype<ModelComponent>("plane"));
    Archetype<ModelComponent>& planeCompType = modelFactory->GetArchetype("plane");
    ModelComponent& planeModelComp = planeCompType.GetBase();
    planeModelComp.SetModel("plane");

    modelFactory->AddArchetype("dirLight", Archetype<ModelComponent>("dirLight"));
    Archetype<ModelComponent>& dirLightCompType = modelFactory->GetArchetype("dirLight");
    ModelComponent& dirLightModelComp = dirLightCompType.GetBase();
    dirLightModelComp.SetModel("sphere");


    
    
    
    object_factory_.AddArchetype("dirLight", Archetype<GameObject>("dirLight"));
    Archetype<GameObject>& dirLight = object_factory_.GetArchetype("dirLight");
    GameObject& dirLightObject = dirLight.GetBase();
    Component* lightComp = comp_manager_->CreateComponent("LightComponent",
                                                          "dirLight",
                                                          &dirLightObject);
    dirLightObject.AddComponent(lightComp);




    WickedSick::Graphics* gSys = (WickedSick::Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);

    object_factory_.AddArchetype("bunny", Archetype<GameObject>("bunny") );
    Archetype<GameObject>& bunny = object_factory_.GetArchetype("bunny");
    GameObject& bunnyObject = bunny.GetBase();
    Component* bunnyComp = comp_manager_->CreateComponent("ModelComponent", 
                                                          "bunny", 
                                                          &bunnyObject);
    bunnyObject.AddComponent(bunnyComp);
    Component* bunnyPhysicsComp = comp_manager_->CreateComponent( "PhysicsComponent",
                                                                  "bunny",
                                                                  &bunnyObject);
    bunnyObject.AddComponent(bunnyPhysicsComp);





    object_factory_.AddArchetype("plane", Archetype<GameObject>("plane"));
    Archetype<GameObject>& plane = object_factory_.GetArchetype("plane");
    GameObject& planeObject = plane.GetBase();
    Component* planeComp = comp_manager_->CreateComponent("ModelComponent",
                                                          "plane",
                                                          &planeObject);




    modelFactory->AddArchetype("box", Archetype<ModelComponent>("box"));
    Archetype<ModelComponent>& cubeCompType = modelFactory->GetArchetype("box");
    ModelComponent& cubeModelComp = cubeCompType.GetBase();
    cubeModelComp.SetModel("box");



    object_factory_.AddArchetype("box", Archetype<GameObject>("box") );
    Archetype<GameObject>& cube = object_factory_.GetArchetype("box");
    GameObject& cubeObject = cube.GetBase();
    Component* cubeComp = comp_manager_->CreateComponent( "ModelComponent",
                                                          "box",
                                                          &cubeObject);
    cubeObject.AddComponent(cubeComp);

    
    Component* cubePhysicsComp = comp_manager_->CreateComponent("PhysicsComponent",
                                                                "box",
                                                                &cubeObject);
    cubeObject.AddComponent(cubePhysicsComp);



    modelFactory->AddArchetype("sphere", Archetype<ModelComponent>("sphere"));
    Archetype<ModelComponent>& sphereCompType = modelFactory->GetArchetype("sphere");
    ModelComponent& sphereModelComp = sphereCompType.GetBase();
    sphereModelComp.SetModel("sphere");


    object_factory_.AddArchetype("sphere", Archetype<GameObject>("sphere"));
    Archetype<GameObject>& sphere = object_factory_.GetArchetype("sphere");
    GameObject& sphereObject = sphere.GetBase();
    Component* sphereComp = comp_manager_->CreateComponent("ModelComponent",
                                                           "sphere",
                                                           &sphereObject);
    sphereObject.AddComponent(sphereComp);

    Component* spherePhysicsComp = comp_manager_->CreateComponent("PhysicsComponent",
                                                                  "sphere",
                                                                  &sphereObject);
    sphereObject.AddComponent(spherePhysicsComp);

    static_cast<PhysicsComponent*>(spherePhysicsComp)->GetRigidBody()->SetGravityScalar(0.0f);

    Component* sphereOrbitComp = comp_manager_->CreateComponent("OrbitComponent",
                                                                "sphere",
                                                                &sphereObject);
    sphereObject.AddComponent(sphereOrbitComp);


    ComponentFactory<CameraController>* controllerFactory = (ComponentFactory<CameraController>*)comp_manager_->GetFactory("CameraController");
    controllerFactory->AddArchetype("camera", Archetype<CameraController>("camera"));



    ComponentFactory<CameraComponent>* cameraFactory = (ComponentFactory<CameraComponent>*)comp_manager_->GetFactory("CameraComponent");
    cameraFactory->AddArchetype("camera", Archetype<CameraComponent>("camera"));
    Archetype<CameraComponent>& cameraCompType = cameraFactory->GetArchetype("camera");
    CameraComponent& cameraTemplateComp = cameraCompType.GetBase();
    cameraTemplateComp.SetLookAt(Vector3(0.0f,0.0f,0.0f));

    object_factory_.AddArchetype("camera", Archetype<GameObject>("camera"));
    Archetype<GameObject>& camera = object_factory_.GetArchetype("camera");
    GameObject& cameraObject = camera.GetBase();
    Component* cameraComp = comp_manager_->CreateComponent("CameraComponent",
                                                           "camera",
                                                           &cameraObject);
    cameraObject.AddComponent(cameraComp);

    Component* cameraController = comp_manager_->CreateComponent("CameraController",
                                                                 "camera",
                                                                 &cameraObject);
    cameraObject.AddComponent(cameraController);
    Component* lightModelComp = comp_manager_->CreateComponent("ModelComponent",
                                                               "dirLight",
                                                               &dirLightObject);
    dirLightObject.AddComponent(lightModelComp);

  }

  void ObjectFactory::BuildScene()
  {

    GameObject* camera = CloneArchetype("camera");
    Transform* cameraTr = (Transform*)camera->GetComponent(CT_Transform);
    cameraTr->SetRotation(0.0f, PI / 4.0f, 0.0f);
    cameraTr->SetPosition(0.0f, 10.0f, 10.0f);
    CameraComponent* cameraComp = (CameraComponent*)camera->GetComponent(CT_CameraComponent);
    cameraComp->SetLookAt(Vector3(0.0f, 0.0f, 0.0f));

    


    Graphics* graphics = (Graphics*)Engine::GetCore()->GetSystem(ST_Graphics);
    graphics->GetCamera()->SetSource((CameraComponent*)cameraComp);






    GameObject* plane = CloneArchetype("plane");
    Transform* planeTr = (Transform*) plane->GetComponent(CT_Transform);
    //bunnyTr->SetRotation(0.0f, PI/4.0f, 0.0f);
    planeTr->SetPosition(0.0f, -15.0f, 0.0f);
    planeTr->SetScale(15.0f);
    ModelComponent* planeModel = (ModelComponent*) plane->GetComponent(CT_ModelComponent);
    planeModel->SetDrawType(DrawType::Default);
    planeModel->SetShader("pixelblinn");
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



    GameObject* bunny = CloneArchetype("box");
    Transform* bunnyTr = (Transform*)bunny->GetComponent(CT_Transform);
    //bunnyTr->SetRotation(0.0f, PI/4.0f, 0.0f);
    bunnyTr->SetPosition(3.0f, 4.0f, 0.0f);
    bunnyTr->SetScale(2.0f);
    ModelComponent* bunnyModel = (ModelComponent*) bunny->GetComponent(CT_ModelComponent);
    bunnyModel->SetDrawType(DrawType::Default);
    bunnyModel->SetShader("pixelblinn");
    bunnyModel->SetTexture("cat_test");
    Material& mat = bunnyModel->GetMaterial();

    mat.ambientColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    mat.specularColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
    mat.emissiveColor = Vector4(0.0f, 0.0f, 0.0f, 1.0f);
    mat.diffuseColor = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

    mat.ambientConstant =  0.3f;
    mat.specularConstant = 1.0f;
    mat.emissiveConstant = 0.3f;
    mat.diffuseConstant =  0.5f;
    mat.shininess = 20.0f;

    PhysicsComponent* bunnyPhysics = (PhysicsComponent*)bunny->GetComponent(CT_PhysicsComponent);
    RigidBody* bunnyBody = bunnyPhysics->GetRigidBody();
    bunnyBody->SetGravityScalar(0.0f);
    
    
    Vector3 axis = Vector3(0.0f, 1.0f, 0.0f);
    for(int i = 0; i < 8; ++i)
    {
      Vector3 fromBunny = Vector3(8.0f, 5.0f, 8.0f);
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
        info.point.ambientIntensity.Zero();
        info.point.diffuseIntensity.Zero();
        info.point.specularIntensity.Zero();
      }
      else
      {
        info.point.ambientIntensity = {0.2f, 0.2f, 0.2f};
        info.point.diffuseIntensity = {1.0f, 1.0f, 1.0f};
        info.point.specularIntensity = {1.0f, 1.0f, 1.0f};
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

