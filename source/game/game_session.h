#ifndef AAIRACE_SOURCE_GAME_SESSION_H_
#define AAIRACE_SOURCE_GAME_SESSION_H_

#include <memory>
#include <vector>

#include "game_business_logic.h"
#include "ui/game_window.h"

class EnemyAI;
class LevelManager;

class GameSession {
 public:
  GameSession() = delete;
  GameSession(const GameSession& game_session) = delete;
  GameSession(GameSession&& game_session) = delete;
  GameSession& operator=(const GameSession& game_session) = delete;
  GameSession& operator=(GameSession&& game_session) = delete;

  GameSession(LevelManager* const level_manager, 
              const GameWindowContext& game_window_context,
              int enemies_count,
              int city_car_count,
              const std::vector<std::shared_ptr<EnemyAI>>& enemies_ai);

  void NotifyGameCycleElapsed(float elapsed_time, const UserControllersContext& context);
  void DrawEntities();
  
  bool is_game_session_ended() const;
 private:
  GameBusinessLogic game_business_logic_;
  LevelManager* const level_manager_;
  float game_aiio_scan_timer_;
  bool collected_aiio_data_sended_;
  std::vector<AIIOData> collected_aiio_data_;
};

#endif  // AAIRACE_SOURCE_GAME_SESSION_H_