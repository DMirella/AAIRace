#include "user_profile.h"

#ifdef _WIN32
  #include <direct.h>
#else
  #include <sys/stat.h> // mkdir
#endif

#include <cstring>
#include <fstream>
#include <iostream>

namespace {
const std::string gProfilesFolderPath = "profiles";
const std::string gProfileNameFileTitle = "user";
const std::string gProfileFileFormat = "aairp";
const int kNameBufferSize = 256;
}  // namespace

namespace game {
UserProfile::UserProfile(const ui::GameWindowContext& game_window_context)
    : name_("")
    , level_manager_(game_window_context, LevelManager::kMinimumCountUnlockedLevels) {
}

void UserProfile::SetName(const std::string& name) {
  name_ = name;
}

std::string UserProfile::name() const {
  return name_;
}

LevelManager& UserProfile::GetLevelManager() {
  return level_manager_;
}

bool UserProfile::LoadFromConfigFile(const std::string& user_name) {
  const std::string kProfileFolder = gProfilesFolderPath + "/" + user_name;
  std::ifstream f(kProfileFolder + "/" + gProfileNameFileTitle + "." + gProfileFileFormat);
  if (!f.fail()) {
    f >> name_;
    f.close();
    level_manager_.LoadFromFolder(kProfileFolder);
    return true;
  }
  return false;
}

void UserProfile::SaveToConfigFile() const {
  const std::string kProfileFolder = gProfilesFolderPath + "/" + name_;
#if defined _MSC_VER
  _mkdir(gProfilesFolderPath.c_str());
  _mkdir((gProfilesFolderPath + "/" + name_).c_str());
#elif defined __GNUC__
  mkdir(gProfilesFolderPath.c_str(), 0777);
  mkdir((gProfilesFolderPath + "/" + name_).c_str(), 0777);
#endif
  std::ofstream f(kProfileFolder + "/" + gProfileNameFileTitle + "." + gProfileFileFormat);
  f << name_ << '\n';
  f.close();
  level_manager_.SaveToFolder(kProfileFolder);
}

void UserProfile::Reset() {
  if (!name_.empty()) {
    SaveToConfigFile();
  }
  name_.clear();
  level_manager_.Reset();
}

bool UserProfile::CheckIfConfigExist(const std::string& user_name) {
  const std::string kProfileFolder = gProfilesFolderPath + "/" + user_name;
  std::ifstream f(kProfileFolder + "/" + gProfileNameFileTitle + "." + gProfileFileFormat);
  return f.good();
}
}  // namespace game
