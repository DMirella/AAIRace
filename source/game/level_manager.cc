#include "level_manager.h"

#include <cassert>
#include <iostream>

#include "ai/enemy_ai.h"
#include "game_session.h"

namespace {
const int gCollectedDataLevelsSlidingWindowSize = 3;
const int gLevelsCount = 30;
const int gEnemiesCount = 4;
}  // namespace


LevelManager::LevelManager(const GameWindowContext& game_window_context, int count_unlocked_levels)
    : game_window_context_(game_window_context)
    , current_active_level_(-1)
    , count_unlocked_levels_(count_unlocked_levels) {
  collected_user_aiio_data_.resize(gLevelsCount);
  for (int i = 0; i < gLevelsCount; i++) {
    collected_user_aiio_data_[i] = std::make_shared<std::vector<AIIOData>>();
  }
  enemies_ai_.resize(gLevelsCount);
  for (int i = 0; i < gLevelsCount; i++) {
    enemies_ai_[i] = std::make_shared<EnemyAI>(collected_user_aiio_data_[i]);
  }
}

int LevelManager::count_unlocked_level() const {
  return count_unlocked_levels_;
}

std::shared_ptr<GameSession> LevelManager::GenerateGameSession(int game_level) {
  assert(game_level <= gLevelsCount);
  current_active_level_ = game_level;
  const int kMaxCityCarCount = 300;
  const int kCityCarsCount = (game_level * 18 > kMaxCityCarCount)? kMaxCityCarCount : game_level * 18;
  return std::make_shared<GameSession>(this, game_window_context_, gEnemiesCount, kCityCarsCount, 
                                       std::vector<std::shared_ptr<EnemyAI>>(gEnemiesCount, enemies_ai_[game_level - 1]));
}

void LevelManager::NotifyCurrentLevelEnds(const std::vector<AIIOData>& collected_aiio_data) {
  if (current_active_level_ == count_unlocked_levels_) {
    current_active_level_ = -1;
    
    // TODO: if will be choosen neural network for AI simulate use it
    if (count_unlocked_levels_ < gLevelsCount) {
      count_unlocked_levels_++;
      auto& current_filling_collected_user_data = collected_user_aiio_data_[count_unlocked_levels_ - 1];
      *current_filling_collected_user_data = FilterAIIOData(collected_aiio_data);
      int to = std::max(count_unlocked_levels_ - 2 - gCollectedDataLevelsSlidingWindowSize, 0);
      for (int i = count_unlocked_levels_ - 2; i >= to; i--) {
        current_filling_collected_user_data->insert(
            current_filling_collected_user_data->end(),
            collected_user_aiio_data_[i]->begin(),
            collected_user_aiio_data_[i]->end());
      }
      enemies_ai_[count_unlocked_levels_] = std::make_shared<EnemyAI>(current_filling_collected_user_data);
      // enemies_ai_[count_unlocked_levels_]->TrainWithData(collected_aiio_data);
    }
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

void LevelManager::LoadFromFile(std::ifstream* const f) {
  *f >> count_unlocked_levels_;
  for (int i = 0; i < count_unlocked_levels_; i++) {
    int current_collected_data_size = 0;
    *f >> current_collected_data_size;
    collected_user_aiio_data_[i]->resize(current_collected_data_size);
    for (auto& current_aiio : *(collected_user_aiio_data_[i])) {
      std::vector<double> vector_input_data(AIInputData::kInputCount, 0.0);
      for (auto& it : vector_input_data) {
        *f >> it;
      }
      std::vector<double> vector_output_data(AIOutputData::kOutputCount, 0.0);
      for (auto& it : vector_output_data) {
        *f >> it;
      }
      current_aiio.input = TransformVectorToAIInputData(vector_input_data);
      current_aiio.output = TransformVectorToAIOutputData(vector_output_data);
    }
    enemies_ai_[count_unlocked_levels_] = std::make_shared<EnemyAI>(collected_user_aiio_data_[i]);
  }
}

void LevelManager::SaveToFile(std::ofstream* f) const {
  *f << count_unlocked_levels_ << '\n';
  for (int i = 0; i < count_unlocked_levels_; i++) {
    *f << collected_user_aiio_data_[i]->size() << '\n';
    for (const auto& current_aiio : *(collected_user_aiio_data_[i])) {
      auto converted_current_input_data = TransformAIInputDataToVector(current_aiio.input);
      auto converted_current_output_data = TransformAIOutputDataToVector(current_aiio.output);
      for (auto it : converted_current_input_data) {
        *f << it << " ";
      }
      for (auto it : converted_current_output_data) {
        *f << it << " ";
      }
      *f << '\n';
    }
  }
}