#include "Precompiled.h"
#include "SkyboxComponent.h"
#include "Graphics/GraphicsInterface.h"
namespace WickedSick
{
  SkyboxComponent::SkyboxComponent() : Component(CT_SkyboxComponent, nullptr)
  {
  }

  SkyboxComponent::~SkyboxComponent()
  {
  }

  void SkyboxComponent::Initialize()
  {
  }

  void SkyboxComponent::Update(float dt)
  {
    Graphics* graphics = (Graphics*) Engine::GetCore()->GetSystem(ST_Graphics);
    Transform* tr = (Transform*)GetSibling(CT_Transform);
    tr->SetPosition(graphics->GetCamera()->GetPosition());
  }

  void SkyboxComponent::Clone(Component * source)
  {
  }

}
