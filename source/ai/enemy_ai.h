#ifndef AAIRACE_SOURCE_ENEMY_AI_H_
#define AAIRACE_SOURCE_ENEMY_AI_H_

#include <memory>
#include <string>
#include <vector>

#include "ai.h"
#include "ai_types.h"

namespace ai {
class EnemyAI {
 public:
  EnemyAI(const EnemyAI& enemy_ai) = delete;
  EnemyAI(EnemyAI&& enemy_ai) = delete;
  EnemyAI& operator=(const EnemyAI& enemy_ai) = delete;
  EnemyAI& operator=(EnemyAI&& enemy_ai) = delete;

  EnemyAI() = default;

  void Init();
  void TrainWithData(const std::vector<AIIOData>& aiio_data);
  AIOutputData GetOutputData(const AIInputData& input_data);
  void SaveToFile(const std::string& path, const std::string& file_name);
  void InitFromFile(const std::string& path, const std::string& file_name);
 private:
  std::unique_ptr<NeuralNetwork> neural_network_;
};
}  // namespace ai

#endif  // AAIRACE_SOURCE_ENEMY_AI_H_