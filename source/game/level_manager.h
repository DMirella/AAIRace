#ifndef AAIRACE_SOURCE_GAME_LEVEL_MANAGER_H_
#define AAIRACE_SOURCE_GAME_LEVEL_MANAGER_H_

#include <fstream>
#include <list>
#include <memory>
#include <thread>
#include <vector>

#include <ai/ai_types.h>
#include <ui/game_window.h>

namespace ai {
class EnemyAI;
}  // namespace ai

namespace game {
class GameSession;
class LevelManager {
 public:
  LevelManager() = delete;
  LevelManager(const LevelManager& level_manager) = delete;
  LevelManager(LevelManager&& level_manager) = delete;
  LevelManager& operator=(const LevelManager& level_manager) = delete;
  LevelManager& operator=(LevelManager&& level_manager) = delete;

  LevelManager(const ui::GameWindowContext& game_window_context,
               int count_unlocked_levels);
  ~LevelManager();

  int count_unlocked_level() const;
  std::unique_ptr<GameSession> GenerateGameSession(int game_level);
  void NotifyCurrentLevelEnds(const std::vector<ai::AIIOData>& collected_aiio_data);
  
  void LoadFromFolder(const std::string& path);
  void SaveToFolder(const std::string& path);

  void Reset();

  static const int kMinimumCountUnlockedLevels;
  static const int kLevelsCount;
  static const int kEnemiesCount;
 private:
  int current_active_level_;
  int count_unlocked_levels_;
  ui::GameWindowContext game_window_context_;
  std::thread enemies_ai_training_thread_;
  std::list<std::vector<ai::AIIOData>> accumulated_filtered_collected_aiio_data_;
  std::vector<std::vector<std::shared_ptr<ai::EnemyAI>>> enemies_ai_;
};
}  // namespace game

#endif  // AAIRACE_SOURCE_GAME_LEVEL_MANAGER_H_