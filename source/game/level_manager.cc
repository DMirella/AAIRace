#include "level_manager.h"

#include <cassert>
#include <thread>

#include <omp.h>

#include <ai/ai_types.h>
#include <ai/enemy_ai.h>
#include <ui/game_window.h>

#include "game_session.h"

namespace {
const int gCollectedDataLevelsSlidingWindowSize = 3;
const std::string gLevelInfoFile = "levels_info.info";
const std::string gCollectedFilteredDtaInfoFile = "collected_user_data.info";
const std::string gAIFilePrefix = "enemy_ai";
}  // namespace

namespace game {
const int LevelManager::kMinimumCountUnlockedLevels = 1;
const int LevelManager::kLevelsCount = 30;
const int LevelManager::kEnemiesCount = 4;

LevelManager::LevelManager(const ui::GameWindowContext& game_window_context,
                           int count_unlocked_levels)
    : game_window_context_(game_window_context)
    , current_active_level_(-1)
    , count_unlocked_levels_(count_unlocked_levels) {
  accumulated_filtered_collected_aiio_data_.resize(gCollectedDataLevelsSlidingWindowSize);
  enemies_ai_.resize(kLevelsCount);
  for (int i = 0; i < kLevelsCount; i++) {
    enemies_ai_[i] = std::vector<std::shared_ptr<ai::EnemyAI>>(kEnemiesCount);
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j] = std::make_shared<ai::EnemyAI>();
      enemies_ai_[i][j]->Init();
    }
  }
}

LevelManager::~LevelManager() {
  if (enemies_ai_training_thread_.joinable()) {
    enemies_ai_training_thread_.join();
  }
}

int LevelManager::count_unlocked_level() const {
  return count_unlocked_levels_;
}

std::unique_ptr<GameSession> LevelManager::GenerateGameSession(int game_level) {
  assert(game_level <= kLevelsCount);
  current_active_level_ = game_level;
  const int kMaxCityCarCount = 300;
  const int kCityCarsCount 
      = (game_level * 18 > kMaxCityCarCount)? kMaxCityCarCount : game_level * 18;
  
  if (enemies_ai_training_thread_.joinable()) {
    enemies_ai_training_thread_.join();
  }
  
  return std::make_unique<GameSession>(
      this, game_window_context_, kEnemiesCount, kCityCarsCount,
      enemies_ai_[game_level - 1]);
}

void LevelManager::NotifyCurrentLevelEnds(const std::vector<ai::AIIOData>& collected_aiio_data) {
  if (current_active_level_ == count_unlocked_levels_) {
    current_active_level_ = -1;
    if (count_unlocked_levels_ < kLevelsCount) {
      count_unlocked_levels_++;
      auto current_collected_data = ai::FilterAIIOData(collected_aiio_data);
      accumulated_filtered_collected_aiio_data_.pop_front();
      for (const auto& it : accumulated_filtered_collected_aiio_data_) {
        current_collected_data.insert(current_collected_data.end(), it.begin(), it.end());
      }
      accumulated_filtered_collected_aiio_data_.push_back(current_collected_data);
      auto training_function = [this, current_collected_data]() {
        #pragma omp parallel for
        for (int i = 0; i < kEnemiesCount; i++) {
          enemies_ai_[count_unlocked_levels_ - 1][i]->TrainWithData(current_collected_data);
        }
      };
      enemies_ai_training_thread_ = std::thread(training_function);
    }
  }
}

void LevelManager::LoadFromFolder(const std::string& path) {
  if (enemies_ai_training_thread_.joinable()) {
    enemies_ai_training_thread_.join();
  }
  std::ifstream f(path + "/" + gCollectedFilteredDtaInfoFile,
                  std::ios::in | std::ios::binary);
  f.read((char*)&count_unlocked_levels_, sizeof(int));
  accumulated_filtered_collected_aiio_data_.resize(gCollectedDataLevelsSlidingWindowSize);
  auto current_it = accumulated_filtered_collected_aiio_data_.begin();
  for (int i = 0; i < gCollectedDataLevelsSlidingWindowSize; i++) {
    int current_collected_data_size = 0;
    f.read((char*)&current_collected_data_size, sizeof(int));
    current_it->resize(current_collected_data_size);
    for (auto& current_aiio : *(current_it)) {
      std::vector<double> vector_input_data(ai::AIInputData::kInputCount, 0.0);
      for (auto& it : vector_input_data) {
        f.read((char*)&it, sizeof(double));
      }
      std::vector<double> vector_output_data(ai::AIOutputData::kOutputCount, 0.0);
      for (auto& it : vector_output_data) {
        f.read((char*)&it, sizeof(double));
      }
      current_aiio.input = ai::TransformVectorToAIInputData(vector_input_data);
      current_aiio.output = ai::TransformVectorToAIOutputData(vector_output_data);
    }
    ++current_it;
  }
  f.close();

  enemies_ai_.resize(kLevelsCount);
  for (int i = 0; i < kLevelsCount; i++) {
    enemies_ai_[i] = std::vector<std::shared_ptr<ai::EnemyAI>>(kEnemiesCount);
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j] = std::make_shared<ai::EnemyAI>();
    }
  }
  for (int i = 0; i < count_unlocked_levels_; i++) {
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j]->InitFromFile(path, gAIFilePrefix + std::to_string(i) + std::to_string(j));
    }
  }
  for (int i = count_unlocked_levels_; i < kLevelsCount; i++) {
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j]->Init();
    }
  }
}

void LevelManager::Reset() {
  if (enemies_ai_training_thread_.joinable()) {
    enemies_ai_training_thread_.join();
  }
  accumulated_filtered_collected_aiio_data_.clear();
  accumulated_filtered_collected_aiio_data_.resize(gCollectedDataLevelsSlidingWindowSize);
  current_active_level_ = -1;
  count_unlocked_levels_ = kMinimumCountUnlockedLevels;
  enemies_ai_.clear();
  enemies_ai_.resize(kLevelsCount);
  for (int i = 0; i < kLevelsCount; i++) {
    enemies_ai_[i] = std::vector<std::shared_ptr<ai::EnemyAI>>(kEnemiesCount);
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j] = std::make_shared<ai::EnemyAI>();
      enemies_ai_[i][j]->Init();
    }
  }
}

void LevelManager::SaveToFolder(const std::string& path) {
  if (enemies_ai_training_thread_.joinable()) {
    enemies_ai_training_thread_.join();
  }
  std::ofstream f(path + "/" + gCollectedFilteredDtaInfoFile,
                  std::ios::out | std::ios::binary);
  f.write((char*)&count_unlocked_levels_, sizeof(int));
  for (const auto& it : accumulated_filtered_collected_aiio_data_) {
    int data_size = it.size();
    f.write((char*)&data_size, sizeof(int));
    for (const auto& current_aiio : it) {
      auto converted_current_input_data = TransformAIInputDataToVector(current_aiio.input);
      auto converted_current_output_data = TransformAIOutputDataToVector(current_aiio.output);
      for (auto it : converted_current_input_data) {
        f.write((char*)&it, sizeof(double));
      }
      for (auto it : converted_current_output_data) {
        f.write((char*)&it, sizeof(double));
      }
    }
  }
  f.close();
  for (int i = 0; i < count_unlocked_levels_; i++) {
    for (int j = 0; j < kEnemiesCount; j++) {
      enemies_ai_[i][j]->SaveToFile(path, gAIFilePrefix + std::to_string(i) + std::to_string(j));
    }
  }
}
}  // namespace game