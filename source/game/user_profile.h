#ifndef AAIRACE_SOURCE_GAME_USER_PROFILE_H_
#define AAIRACE_SOURCE_GAME_USER_PROFILE_H_

#include <memory>
#include <string>

#include "level_manager.h"

struct GameWindowContext;
class UserProfile {
 public:
  UserProfile(const GameWindowContext& game_window_context);
  
  void SetName(const std::string& name);
  std::string name() const;
  LevelManager& level_manager();
  bool LoadFromConfigFile(const std::string& user_name);
  void SaveToConfigFile() const;
  static bool CheckIfConfigExist(const std::string& user_name);
 private:
  std::string name_;
  LevelManager level_manager_;
};

#endif  // AAIRACE_SOURCE_GAME_USER_PROFILE_H_