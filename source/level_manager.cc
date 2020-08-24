#include "level_manager.h"

LevelManager::LevelManager(const GameWindowContext& game_window_context)
    : game_window_context_(game_window_context) {
}

GameSession LevelManager::GenerateGameSession(int game_level) {
  const int kEnemiesCount = 4;
  std::vector<EnemyAI> enemies_ai;
  for (int i = 0; i < kEnemiesCount; i++) {
    enemies_ai.push_back(EnemyAI());
  }
  return GameSession(this, game_window_context_, kEnemiesCount, 250, enemies_ai);
}

void LevelManager::NotifyAIIODataCollected(const std::vector<AIIOData>& collected_aiio_data) {

}