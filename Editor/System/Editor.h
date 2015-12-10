#pragma once

#include "Core/CoreInterface.h"

#include "anttweakbar/include/AntTweakBar.h"
#include "TweakBarCallbacks.h"
namespace WickedSick
{

  

  class GameObject;
  class Event;
  class ParticleEditor;
  class Editor : public System
  {
    public:
      Editor();
      ~Editor();

      void Initialize() override;
      bool Load() override;
      bool Reload() override;
      void Update(float dt) override;
      void ReceiveMessage(Event* letter) override;

      GameObject* GetCurrentSelected();

      std::string GetCurrentArchetype();
      void SetCurrentArchetype(const std::string& archetype);

      void UpdateSelected();

      void Select(GameObject* target);
      void Select(int id);
      void Deselect(GameObject* target);
      void Deselect(int id);
      void Deselect();
      CallbackInfo& GetCBInfo();
      int GetSelectedId();
      ParticleEditor* GetParticleEditor();

    private:
      void add_members(CallbackInfo& cbInfo, Reflection::Member* base);
      void draw_ui();
      std::set<int> selected_;
      int selected_id_;
      int archetype_index_;

      bool show_particle_editor_;
      ParticleEditor* particle_editor_;



      TwType archetype_enum_;
      std::string archetype_to_clone_;
      std::vector<TwEnumVal> archetypeEnums;

      std::list<CallbackInfo> per_object_info_;
      
      CallbackInfo cb_info_;

      TwBar* object_ui_;
      TwBar* scene_ui_;

  };
}
