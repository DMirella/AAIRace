#include "level_manager.h"

#include <map>

#include "ai/enemy_ai.h"
#include "game_session.h"

namespace {
const int gEnemiesCount = 4;
}  // namespace

LevelManager::LevelManager(const GameWindowContext& game_window_context)
    : game_window_context_(game_window_context) {
  enemies_ai_.resize(gEnemiesCount);
  for (int i = 0; i < gEnemiesCount; i++) {
    enemies_ai_[i] = std::make_shared<EnemyAI>();
  }
}

std::shared_ptr<GameSession> LevelManager::GenerateGameSession(int game_level) {
  const int kCityCarsCount = game_level * 9;
  return std::make_shared<GameSession>(this, game_window_context_, gEnemiesCount, kCityCarsCount, enemies_ai_);
}

void LevelManager::NotifyAIIODataCollected(const std::vector<AIIOData>& collected_aiio_data) {
  for (int i = 0; i < gEnemiesCount; i++) {
    //enemies_ai_[i].reset();
    //enemies_ai_[i] = std::make_shared<EnemyAI>();
  }
  for (const auto &it : enemies_ai_) {
    it->TrainWithData(collected_aiio_data);
  }
  /*for (int i = 0; i < 1; i++) {
    std::vector<EnemyAI> new_enemies_ai;
    std::vector<std::pair<float, int>> enemies_ai_marks_;
    for (int j = 0; j < gEnemiesCount; j++) {
      float current_mark = 0.0f;
      for (const auto& it : collected_aiio_data) {
        AIOutputData output = enemies_ai_[j].GetOutputData(it.input);
        int count_singles 
            = (output.is_accelerate_action_active == it.output.is_accelerate_action_active)
            + (output.is_car_slow_action_active == it.output.is_car_slow_action_active)
            + (output.is_car_turn_left_action_active == it.output.is_car_turn_left_action_active)
            + (output.is_car_turn_right_action_active == it.output.is_car_turn_right_action_active);
        current_mark += count_singles;
      }
      enemies_ai_marks_.push_back(std::make_pair(current_mark, j));
    }
    std::sort(enemies_ai_marks_.begin(), enemies_ai_marks_.end());

    new_enemies_ai.push_back(enemies_ai_[enemies_ai_marks_.back().second]);
    for (int j = gEnemiesCount - 2; j >= 0; j++) {
      EnemyAI tmp;
      //tmp.neural_network_ = NeuralNetwork::MergeNeuralNetworks(enemies_ai_[enemies_ai_marks_.back().second].neural_network_, enemies_ai_[j].neural_network_);
      new_enemies_ai.emplace_back(tmp);
    }
    enemies_ai_ = std::move(new_enemies_ai);
  }*/
}