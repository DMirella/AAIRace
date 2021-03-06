#include "game_session.h"

#include <iostream>

#include "level_manager.h"

namespace game {
GameSession::GameSession(LevelManager* const level_manager, 
                         const ui::GameWindowContext& game_window_context,
                         int enemies_count,
                         int city_car_count,
                         const std::vector<std::shared_ptr<ai::EnemyAI>>& enemies_ai)
    : game_business_logic_(game_window_context, enemies_count, city_car_count, enemies_ai)
    , level_manager_(level_manager)
    , collected_aiio_data_sended_(false)
    , collected_aiio_data_{} {
}

void GameSession::NotifyGameCycleElapsed(float elapsed_time, 
                                         const ui::UserControllersContext& context) {
  const float kGameAIIOScanRate = 1000.0f;  // ms
  game_business_logic_.NotifyGameCycleElapsed(elapsed_time, context);
  game_aiio_scan_timer_ += elapsed_time;
  if (game_aiio_scan_timer_ >= kGameAIIOScanRate) {
    collected_aiio_data_.emplace_back(game_business_logic_.GetAIIODataRegardingToHeroCar());
  }
  if (!collected_aiio_data_sended_ && 
      game_business_logic_.is_game_ended()) {
    collected_aiio_data_sended_ = true;
    level_manager_->NotifyCurrentLevelEnds(collected_aiio_data_);
  }
}

void GameSession::DrawEntities() {
  game_business_logic_.DrawEntities();
}

bool GameSession::is_game_session_ended() const {
  return game_business_logic_.is_game_ended();
}
}  // namespace game