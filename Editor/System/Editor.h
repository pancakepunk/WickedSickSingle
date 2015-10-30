#pragma once

#include "Core/CoreInterface.h"

#include "anttweakbar/include/AntTweakBar.h"
namespace WickedSick
{

  struct CallbackInfo
  {
    Reflection::Metadata* baseData;
    std::vector<Reflection::Member*> members;
  };

  class GameObject;
  class Event;
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
    private:
      void add_members(CallbackInfo& cbInfo, Reflection::Member* base);
      void draw_ui();
      std::set<int> selected_;
      int selected_id_;
      int archetype_index_;
      TwType archetype_enum_;
      std::string archetype_to_clone_;
      std::vector<TwEnumVal> archetypeEnums;

      std::vector<CallbackInfo*> per_object_info_;

      CallbackInfo cb_info_;

      TwBar* object_ui_;
      TwBar* scene_ui_;

  };
}
