#pragma once



#include "InputBuffer.h"

#include "Utility/UtilityInterface.h"
#include "Math/MathInterface.h"
#include <set>


namespace WickedSick
{
  enum SystemType;

  struct Keybind
  {
    InputType::Enum type;
    int primaryId;
    std::set<int> ids;
    std::set<ModifierType::Enum> modifiers;
  };

  class InputHandler
  {
    public:

      InputHandler();

      void Update(float dt);
      void SetActive(bool active);
      void Load(const std::string& filename);
      void Save(const std::string& filename);

      bool Check(const std::string& identifier);
      Vector2i GetMousePos();
      int GetScrollPos();

      void UpdateInput(const InputBuffer& buf);

      std::vector<Keybind> get_keys(const std::string& identifier);
    private:
      //                  command     key to bind
      HashMap<std::string, Keybind> inputs_;
      std::set<std::string> active_inputs_;
      std::vector<Key> keys;
      Vector2i cur_mouse_pos_;
      int cur_wheel_pos_;
      bool current_modifiers_[ModifierType::Count];

      bool active_;
  };
}
