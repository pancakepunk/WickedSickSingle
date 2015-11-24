#include "Precompiled.h"
#include "LogicPrecompiled.h"
#include "Core/CoreInterface.h"
#include "Graphics/GraphicsInterface.h"
#include "Window/WindowInterface.h"
#include "Input/InputInterface.h"
#include "Utility/UtilityInterface.h"
#include "Physics/PhysicsInterface.h"
#include "Editor/EditorInterface.h"

#include "Testing/TestingInterface.h"
#include "ObjectFactory/ObjectFactoryInterface.h"

#include "EventSystem/EventSystemInterface.h"

int main(void)
{

  WickedSick::Engine* engine = new WickedSick::Engine();

  engine->AddSystem(new WickedSick::Window());
  engine->AddSystem(new WickedSick::Graphics());
  engine->AddSystem(new WickedSick::Input());
  engine->AddSystem(new WickedSick::EventSystem());
  engine->AddSystem(new WickedSick::ObjectFactory());
  engine->AddSystem(new WickedSick::Physics());
  engine->AddSystem(new WickedSick::Editor());
  

  engine->Initialize();
  engine->Load();

  WickedSick::ObjectFactory* fact = (WickedSick::ObjectFactory*)engine->GetSystem(WickedSick::ST_ObjectFactory);
  fact->CreateArchetypes();
  fact->BuildScene();
  WickedSick::Physics* pSys = (WickedSick::Physics*)engine->GetSystem(WickedSick::ST_Physics);
  pSys->GetScene()->LoadScene();
  while (engine->IsActive())
  {
    engine->Update();
  }

  delete engine;

  return 0;
}


