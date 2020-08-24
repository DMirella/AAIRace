#ifndef AAIRACE_SOURCE_LEVEL_MANAGER_H_
#define AAIRACE_SOURCE_LEVEL_MANAGER_H_

#include "game_session.h"
#include "game_window.h"

class LevelManager {
 public:
  LevelManager(const GameWindowContext& game_window_context);
  GameSession GenerateGameSession(int game_level);

  void NotifyAIIODataCollected(const std::vector<AIIOData>& collected_aiio_data);
 private:
  GameWindowContext game_window_context_;
};

#endif  // AAIRACE_SOURCE_LEVEL_MANAGER_H_