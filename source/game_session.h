#ifndef AAIRACE_SOURCE_GAME_SESSION_H_
#define AAIRACE_SOURCE_GAME_SESSION_H_

#include <memory>
#include <vector>

#include "enemy_ai.h"
#include "game_business_logic.h"
#include "game_window.h"

class LevelManager;

class GameSession {
 public:
  GameSession(LevelManager* const level_manager, 
              const GameWindowContext& game_window_context,
              int enemies_count,
              int city_car_count,
              const std::vector<EnemyAI>& enemies_ai);
  ~GameSession();

  void NotifyGameCycleElapsed(float elapsed_time);
  void DrawEntities();

  bool is_game_session_ended() const;
 private:
  GameBusinessLogic game_business_logic_;
  LevelManager* const level_manager_;
  float game_aiio_scan_timer_;

  std::vector<AIIOData> collected_aiio_data_;
};

#endif  // AAIRACE_SOURCE_GAME_SESSION_H_