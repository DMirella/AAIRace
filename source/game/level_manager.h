#ifndef AAIRACE_SOURCE_LEVEL_MANAGER_H_
#define AAIRACE_SOURCE_LEVEL_MANAGER_H_

#include <memory>

#include "game_session.h"
#include "ui/game_window.h"

class LevelManager {
 public:
  LevelManager(const GameWindowContext& game_window_context);
  GameSession GenerateGameSession(int game_level);

  void NotifyAIIODataCollected(const std::vector<AIIOData>& collected_aiio_data);
 private:
  GameWindowContext game_window_context_;
  std::vector<std::shared_ptr<EnemyAI>> enemies_ai_;
};

#endif  // AAIRACE_SOURCE_LEVEL_MANAGER_H_