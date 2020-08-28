#ifndef AAIRACE_SOURCE_LEVEL_MANAGER_H_
#define AAIRACE_SOURCE_LEVEL_MANAGER_H_

#include <fstream>
#include <memory>
#include <vector>

#include "ai/ai_types.h"
#include "ui/game_window.h"

class EnemyAI;
class GameSession;
class LevelManager {
 public:
  LevelManager(const GameWindowContext& game_window_context, int count_unlocked_levels);

  int count_unlocked_level() const;
  std::shared_ptr<GameSession> GenerateGameSession(int game_level);
  void NotifyCurrentLevelEnds(const std::vector<AIIOData>& collected_aiio_data);
  
  void LoadFromFile(std::fstream* const f);
  void SaveToFile(std::fstream* const f) const;

  static const int kLevelsCount;
  static const int kEnemiesCount;
 private:
  int current_active_level_;
  int count_unlocked_levels_;
  GameWindowContext game_window_context_;

  std::vector<std::shared_ptr<std::vector<AIIOData>>> collected_user_aiio_data_;
  std::vector<std::shared_ptr<EnemyAI>> enemies_ai_;
};

#endif  // AAIRACE_SOURCE_LEVEL_MANAGER_H_