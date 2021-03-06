#ifndef AAIRACE_SOURCE_GAME_USER_PROFILE_H_
#define AAIRACE_SOURCE_GAME_USER_PROFILE_H_

#include <memory>
#include <string>

#include "level_manager.h"

namespace ui {
struct GameWindowContext;
}  // namespace ui

namespace game {
class UserProfile {
 public:
  UserProfile() = delete;
  UserProfile(const UserProfile& user_profile) = delete;
  UserProfile(UserProfile&& user_profile) = delete;
  UserProfile& operator=(const UserProfile& user_profile) = delete;
  UserProfile& operator=(UserProfile&& user_profile) = delete;

  UserProfile(const ui::GameWindowContext& game_window_context);
  
  void SetName(const std::string& name);
  std::string name() const;
  LevelManager& GetLevelManager();
  bool LoadFromConfigFile(const std::string& user_name);
  void SaveToConfigFile();
  void Reset();

  static bool CheckIfConfigExist(const std::string& user_name);
 private:
  std::string name_;
  LevelManager level_manager_;
};
}  // namespace game

#endif  // AAIRACE_SOURCE_GAME_USER_PROFILE_H_