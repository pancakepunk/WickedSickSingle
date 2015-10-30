#pragma once

#include "Core/CoreInterface.h"
#include "Utility/UtilityInterface.h"
#include "Math/MathInterface.h"



namespace WickedSick
{

  class GameObject;
  class ComponentManager;
  class ObjectFactory : public System
  {
  public:
    ObjectFactory();
    ~ObjectFactory();
    void Initialize();
    bool Load();
    bool Reload();
    void Update(float dt);
    void ReceiveMessage(Event * msg);
    GameObject* CloneArchetype(const std::string& name);
    GameObject* MakeBlank();

    ComponentManager* GetComponentManager();
    
    
    std::unordered_map<std::string, Archetype<GameObject> >& GetArchetypes();
    GameObject* GetObject(int id);
    //testing and hardcoded
    void CreateArchetypes();
    void BuildScene();
    ///////////////////////

  private:


    ComponentManager*   comp_manager_;
    Factory<GameObject> object_factory_;

    std::unordered_map<int, GameObject*> game_objects_;

    unsigned objects_created_;

    //static MemoryManager<GameObject> game_object_allocator_;


  };
}
