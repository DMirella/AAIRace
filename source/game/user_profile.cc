#include "user_profile.h"

#include <direct.h>
#include <fstream>
#include <iostream>

namespace {
const std::string gProfilesFolderPath = "profiles";
const std::string gProfileFileFormat = "txt";
}  // namespace

UserProfile::UserProfile(const GameWindowContext& game_window_context)
    : level_manager_(game_window_context, 1) {
}

void UserProfile::SetName(const std::string& name) {
  name_ = name;
}

std::string UserProfile::name() const {
  return name_;
}

LevelManager& UserProfile::level_manager() {
  return level_manager_;
}

bool UserProfile::LoadFromConfigFile(const std::string& user_name) {
  std::ifstream f(gProfilesFolderPath + "/" + user_name + "." + gProfileFileFormat);
  if (!f.fail()) {
    f >> name_;
    level_manager_.LoadFromFile(&f);
    f.close();
    return true;
  }
  return false;
}

void UserProfile::SaveToConfigFile() const {
  _mkdir(gProfilesFolderPath.c_str());
  std::ofstream f(gProfilesFolderPath + "/" + name_ + "." + gProfileFileFormat);
  f << name_ << '\n';
  level_manager_.SaveToFile(&f);
  f.close();
}